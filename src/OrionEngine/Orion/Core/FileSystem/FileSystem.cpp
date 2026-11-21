#include "Orion/Core/FileSystem/FileSystem.h"

#include "Orion/Core/Base.h"
#include "Orion/Core/FileSystem/File.h"
#include "Orion/Core/FileSystem/Internal/OpenFileRegistry.h"
#include "Orion/Core/FileSystem/MemoryMountSource.h"
#include "Orion/Core/FileSystem/VirtualPath.h"

#include <functional>
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
        struct MountEntry
        {
            UniquePtr<IMountSource> Source;
            Ref<Internal::OpenFileRegistry> OpenFiles = MakeRef<Internal::OpenFileRegistry>();
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
            Ref<Internal::OpenFileRegistry> OpenFiles;
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

    void FileSystem::Mount(std::string_view name, UniquePtr<IMountSource> source)
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

    UniquePtr<IFile> FileSystem::Open(std::string_view virtualPath, FileMode mode)
    {
        const std::optional<ResolvedPath> resolved = Resolve(virtualPath);
        if (!resolved)
            return nullptr;

        if (mode == FileMode::Write && resolved->Source->IsReadOnly())
        {
            OE_CORE_LOG(Error, "Cannot write to read-only path \"{0}\"", virtualPath);
            return nullptr;
        }

        UniquePtr<IFile> file = resolved->Source->Open(resolved->Relative, mode);
        if (!file)
            return nullptr;
        return MakeUnique<Internal::TrackedFile>(std::move(file), resolved->OpenFiles, std::string(virtualPath));
    }

    bool FileSystem::Exists(std::string_view virtualPath)
    {
        const std::optional<ResolvedPath> resolved = Resolve(virtualPath);
        return resolved && resolved->Source->Exists(resolved->Relative);
    }

    std::optional<std::vector<uint8_t>> FileSystem::ReadBinary(std::string_view virtualPath)
    {
        const UniquePtr<IFile> file = Open(virtualPath, FileMode::Read);
        if (!file)
            return std::nullopt;

        std::vector<uint8_t> data(file->Size());
        if (file->Read(data.data(), data.size()) != data.size())
            return std::nullopt;
        return data;
    }

    std::optional<std::string> FileSystem::ReadText(std::string_view virtualPath)
    {
        const UniquePtr<IFile> file = Open(virtualPath, FileMode::Read);
        if (!file)
            return std::nullopt;

        std::string text(file->Size(), '\0');
        if (file->Read(text.data(), text.size()) != text.size())
            return std::nullopt;
        return text;
    }

    bool FileSystem::WriteBinary(std::string_view virtualPath, std::span<const uint8_t> data)
    {
        const UniquePtr<IFile> file = Open(virtualPath, FileMode::Write);
        return file && file->Write(data.data(), data.size()) == data.size();
    }

    bool FileSystem::WriteText(std::string_view virtualPath, std::string_view text)
    {
        const UniquePtr<IFile> file = Open(virtualPath, FileMode::Write);
        return file && file->Write(text.data(), text.size()) == text.size();
    }

} // namespace Orion
