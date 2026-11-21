#include "Orion/Core/FileSystem/Internal/OpenFileRegistry.h"

#include "Orion/Core/Base.h"

#include <algorithm>

namespace Orion::Internal
{
    void OpenFileRegistry::Add(const IFile* file, std::string virtualPath)
    {
        const std::lock_guard lock(m_Mutex);
        m_Files.emplace_back(file, std::move(virtualPath));
    }

    void OpenFileRegistry::Remove(const IFile* file)
    {
        const std::lock_guard lock(m_Mutex);
        auto it = std::ranges::find_if(m_Files, [file](const auto& entry) { return entry.first == file; });
        OE_CORE_ASSERT(it != m_Files.end(), "Removing a file that was never registered");
        if (it == m_Files.end())
            return;

        *it = std::move(m_Files.back()); // move the last element over this one
        m_Files.pop_back();              // then drop the last slot
    }

    std::vector<std::string> OpenFileRegistry::GetOpenPaths() const
    {
        const std::lock_guard lock(m_Mutex);
        std::vector<std::string> paths;
        paths.reserve(m_Files.size());
        for (const auto& [file, path] : m_Files)
            paths.push_back(path);
        return paths;
    }

    TrackedFile::TrackedFile(UniquePtr<IFile> file, Ref<OpenFileRegistry> registry, std::string virtualPath)
        : m_File(std::move(file)), m_Registry(std::move(registry))
    {
        m_Registry->Add(this, std::move(virtualPath));
    }

    TrackedFile::~TrackedFile()
    {
        m_Registry->Remove(this); // runs before m_File is destroyed
    }

    size_t TrackedFile::Size() const
    {
        return m_File->Size();
    }

    size_t TrackedFile::Tell() const
    {
        return m_File->Tell();
    }

    bool TrackedFile::Seek(size_t offset)
    {
        return m_File->Seek(offset);
    }

    size_t TrackedFile::Read(void* buffer, size_t size)
    {
        return m_File->Read(buffer, size);
    }

    size_t TrackedFile::Write(const void* data, size_t size)
    {
        return m_File->Write(data, size);
    }

    void TrackedFile::Flush()
    {
        m_File->Flush();
    }
} // namespace Orion::Internal
