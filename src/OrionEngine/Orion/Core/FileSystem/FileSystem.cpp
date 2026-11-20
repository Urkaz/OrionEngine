#include "oepch.h"

#include "Orion/Core/FileSystem/FileSystem.h"

#include "Orion/Core/Core.h"
#include "Orion/Core/FileSystem/File.h"
#include "Orion/Core/FileSystem/MemoryMountSource.h"
#include "Orion/Core/FileSystem/VirtualPath.h"

#include <algorithm>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Orion
{
    namespace
    {
        class OpenFileRegistry
        {
        public:
            void Add(const IFile* file, std::string virtualPath)
            {
                std::lock_guard lock(m_Mutex);
                m_Files.emplace_back(file, std::move(virtualPath));
            }
            void Remove(const IFile* file)
            {
                std::lock_guard lock(m_Mutex);
                auto it = std::ranges::find_if(m_Files, [file](const auto& entry) { return entry.first == file; });
                OE_CORE_ASSERT(it != m_Files.end(), "Removing a file that was never registered");
                if (it == m_Files.end())
                    return;

                *it = std::move(m_Files.back()); // move the last element over this one
                m_Files.pop_back();              // then drop the last slot
            }
            [[nodiscard]] std::vector<std::string> GetOpenPaths() const
            {
                std::lock_guard lock(m_Mutex);
                std::vector<std::string> paths;
                paths.reserve(m_Files.size());
                for (const auto& [file, path] : m_Files)
                    paths.push_back(path);
                return paths;
            }

        private:
            // Files of the same mount can be opened/closed from different threads
            // mutable because GetOpenPaths is const but we need to block it
            mutable std::mutex m_Mutex;
            std::vector<std::pair<const IFile*, std::string>> m_Files;
        };

        // Decorator: registers itself while alive and forwards everything else to the real file
        class TrackedFile final : public IFile
        {
        public:
            TrackedFile(Scope<IFile> file, Ref<OpenFileRegistry> registry, std::string virtualPath)
                : m_File(std::move(file)), m_Registry(std::move(registry))
            {
                m_Registry->Add(this, std::move(virtualPath));
            }
            ~TrackedFile() override
            {
                m_Registry->Remove(this); // runs before m_File is destroyed
            }
            TrackedFile(const TrackedFile&)            = delete;
            TrackedFile& operator=(const TrackedFile&) = delete;
            TrackedFile(TrackedFile&&)                 = delete;
            TrackedFile& operator=(TrackedFile&&)      = delete;

            [[nodiscard]] virtual size_t Size() const override
            {
                return m_File->Size();
            }
            [[nodiscard]] virtual size_t Tell() const override
            {
                return m_File->Tell();
            }
            virtual bool Seek(size_t offset) override
            {
                return m_File->Seek(offset);
            }
            virtual size_t Read(void* buffer, size_t size) override
            {
                return m_File->Read(buffer, size);
            }
            virtual size_t Write(const void* data, size_t size) override
            {
                return m_File->Write(data, size);
            }
            virtual void Flush() override
            {
                m_File->Flush();
            }

        private:
            Scope<IFile> m_File;
            Ref<OpenFileRegistry> m_Registry; // shared: the file may outlive its mount in Release
        };

        struct MountEntry
        {
            Scope<IMountSource> Source;
            Ref<OpenFileRegistry> OpenFiles = CreateRef<OpenFileRegistry>();
        };
        using MountTable = std::unordered_map<std::string, MountEntry, StringHash, std::equal_to<>>;

        MountTable& GetMounts()
        {
            static MountTable mounts;
            return mounts;
        }

        struct ResolvedPath
        {
            IMountSource* Source = nullptr;
            Ref<OpenFileRegistry> OpenFiles;
            std::string Relative;
        };

        // Normalizes the path and finds its mount. nullopt if the path is invalid or nothing is mounted there
        std::optional<ResolvedPath> Resolve(std::string_view virtualPath)
        {
            const std::optional<std::string> normalized = VirtualPath::Normalize(virtualPath);
            if (!normalized)
                return std::nullopt;

            const std::optional<VirtualPath::Parts> parts = VirtualPath::Split(*normalized);
            OE_CORE_ASSERT(parts, "Split cannot fail on a normalized path: \"{}\"", *normalized);

            const auto it = GetMounts().find(parts->Mount);
            if (it == GetMounts().end())
                return std::nullopt;

            // "parts" points into "normalized", which dies here so copy the relative path
            return ResolvedPath{.Source    = it->second.Source.get(),
                                .OpenFiles = it->second.OpenFiles,
                                .Relative  = std::string(parts->Relative)};
        }

        void AssertNoOpenFiles(std::string_view mountName, const MountEntry& entry)
        {
            const std::vector<std::string> openPaths = entry.OpenFiles->GetOpenPaths();
            for (const std::string& path : openPaths)
                OE_CORE_LOG(Error, "Unmounting \"{0}\" with \"{1}\" still open", mountName, path);
            OE_CORE_ASSERT(openPaths.empty(), "Unmounting \"{}\" with open files", mountName);
        }
    } // namespace

    void FileSystem::Mount(std::string_view name, Scope<IMountSource> source)
    {
        auto& mounts = GetMounts();
        if (const auto it = mounts.find(name); it != mounts.end())
        {
            OE_CORE_LOG(Info, "Remounted \"{0}\"", name);
            AssertNoOpenFiles(it->first, it->second);
            it->second.Source = std::move(source); // the same OpenFileRegistry is kept
            return;
        }
        mounts.emplace(std::string(name), MountEntry{.Source = std::move(source)});
    }

    void FileSystem::Unmount(std::string_view name)
    {
        auto& mounts = GetMounts();
        if (const auto it = mounts.find(name); it != mounts.end())
        {
            AssertNoOpenFiles(it->first, it->second);
            mounts.erase(it);
        }
    }

    void FileSystem::UnmountAll()
    {
        for (const auto& [name, entry] : GetMounts())
            AssertNoOpenFiles(name, entry);
        GetMounts().clear();
    }

    Scope<IFile> FileSystem::Open(std::string_view virtualPath, FileMode mode)
    {
        const std::optional<ResolvedPath> resolved = Resolve(virtualPath);
        if (!resolved)
            return nullptr;

        if (mode == FileMode::Write && resolved->Source->IsReadOnly())
        {
            OE_CORE_LOG(Error, "Cannot write to read-only path \"{0}\"", virtualPath);
            return nullptr;
        }

        Scope<IFile> file = resolved->Source->Open(resolved->Relative, mode);
        if (!file)
            return nullptr;
        return CreateScope<TrackedFile>(std::move(file), resolved->OpenFiles, std::string(virtualPath));
    }

    bool FileSystem::Exists(std::string_view virtualPath)
    {
        const std::optional<ResolvedPath> resolved = Resolve(virtualPath);
        return resolved && resolved->Source->Exists(resolved->Relative);
    }

    std::optional<std::vector<uint8_t>> FileSystem::ReadBinary(std::string_view virtualPath)
    {
        const Scope<IFile> file = Open(virtualPath, FileMode::Read);
        if (!file)
            return std::nullopt;

        std::vector<uint8_t> data(file->Size());
        if (file->Read(data.data(), data.size()) != data.size())
            return std::nullopt;
        return data;
    }

    std::optional<std::string> FileSystem::ReadText(std::string_view virtualPath)
    {
        const Scope<IFile> file = Open(virtualPath, FileMode::Read);
        if (!file)
            return std::nullopt;

        std::string text(file->Size(), '\0');
        if (file->Read(text.data(), text.size()) != text.size())
            return std::nullopt;
        return text;
    }

    bool FileSystem::WriteBinary(std::string_view virtualPath, std::span<const uint8_t> data)
    {
        const Scope<IFile> file = Open(virtualPath, FileMode::Write);
        return file && file->Write(data.data(), data.size()) == data.size();
    }

    bool FileSystem::WriteText(std::string_view virtualPath, std::string_view text)
    {
        const Scope<IFile> file = Open(virtualPath, FileMode::Write);
        return file && file->Write(text.data(), text.size()) == text.size();
    }

} // namespace Orion
