#pragma once

#include <cstddef>
#include <cstdint>

namespace Orion
{
    enum class FileMode : uint8_t
    {
        Read,
        Write // Creates the file, or truncates it if it exists
    };

    // An open file. Not thread-safe: it keeps a read/write position
    class IFile
    {
    public:
        IFile()                        = default;
        IFile(const IFile&)            = delete;
        IFile& operator=(const IFile&) = delete;
        IFile(IFile&&)                 = delete;
        IFile& operator=(IFile&&)      = delete;
        virtual ~IFile()               = default;

        [[nodiscard]] virtual size_t Size() const = 0;
        [[nodiscard]] virtual size_t Tell() const = 0;

        // Absolute offset from the start. Returns false (and the position does not change) if offset > Size()
        virtual bool Seek(size_t offset) = 0;

        // Copies up to "size" bytes into "buffer". Returns the number of bytes read, or 0 on failure.
        virtual size_t Read(void* buffer, size_t size) = 0;
        // Copies "size" bytes into file. Returns the number of bytes written, or 0 on failure.
        virtual size_t Write(const void* data, size_t size) = 0;

        // Pushes buffered writes to the storage.
        virtual void Flush() = 0;
    };
} // namespace Orion
