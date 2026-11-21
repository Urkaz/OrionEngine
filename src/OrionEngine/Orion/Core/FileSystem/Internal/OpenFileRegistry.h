#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/FileSystem/File.h"

#include <cstddef>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace Orion::Internal
{
    // Files currently open in a mount, so it can't be unmounted while they are alive
    class OpenFileRegistry
    {
    public:
        void Add(const IFile* file, std::string virtualPath);
        void Remove(const IFile* file);
        [[nodiscard]] std::vector<std::string> GetOpenPaths() const;

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
        TrackedFile(UniquePtr<IFile> file, Ref<OpenFileRegistry> registry, std::string virtualPath);
        ~TrackedFile() override;
        TrackedFile(const TrackedFile&)            = delete;
        TrackedFile& operator=(const TrackedFile&) = delete;
        TrackedFile(TrackedFile&&)                 = delete;
        TrackedFile& operator=(TrackedFile&&)      = delete;

        [[nodiscard]] size_t Size() const override;
        [[nodiscard]] size_t Tell() const override;
        bool Seek(size_t offset) override;
        size_t Read(void* buffer, size_t size) override;
        size_t Write(const void* data, size_t size) override;
        void Flush() override;

    private:
        UniquePtr<IFile> m_File;
        Ref<OpenFileRegistry> m_Registry; // shared: the file may outlive its mount in Release
    };
} // namespace Orion::Internal
