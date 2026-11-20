#include "oepch.h"

#include "AssimpIOSystem.h"

#include "Orion/Core/FileSystem/File.h"
#include "Orion/Core/FileSystem/FileSystem.h"

#include <assimp/types.h>
#include <cstddef>

namespace Orion
{
    size_t AssimpIOStream::Read(void* buffer, size_t size, size_t count)
    {
        if (size == 0)
            return 0;
        return m_File->Read(buffer, size * count) / size;
    }

    aiReturn AssimpIOStream::Seek(size_t offset, aiOrigin origin)
    {
        size_t target = 0;
        switch (origin)
        {
        case aiOrigin_SET:
            // Offset is the absolute position from the start of the file
            target = offset;
            break;
        case aiOrigin_CUR:
            // Relative to the current position (a negative offset moves backwards, see END)
            target = m_File->Tell() + offset;
            break;
        case aiOrigin_END:
            // Callers may pass negative offsets. size_t is unsigned but it arrives as a huge value.
            // The addition causes a wraparound, resulting in a subtraction.
            // If it goes back too far, the result is greater than Size() and IFile::Seek rejects it.
            target = m_File->Size() + offset;
            break;
        default:
            return aiReturn_FAILURE;
        }

        return m_File->Seek(target) ? aiReturn_SUCCESS : aiReturn_FAILURE;
    }

    bool AssimpIOSystem::Exists(const char* path) const
    {
        return FileSystem::Exists(path);
    }

    Assimp::IOStream* AssimpIOSystem::Open(const char* path, const char* mode)
    {
        if (std::string_view(mode).find('w') != std::string_view::npos)
            return nullptr;

        Scope<IFile> file = FileSystem::Open(path, FileMode::Read);
        if (!file)
            return nullptr;
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): Assimp owns it
        return new AssimpIOStream(std::move(file));
    }

    void AssimpIOSystem::Close(Assimp::IOStream* stream)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): created by Open
        delete stream;
    }
} // namespace Orion
