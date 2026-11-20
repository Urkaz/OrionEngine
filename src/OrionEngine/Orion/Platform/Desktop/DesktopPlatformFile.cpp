#include "oepch.h"

#include "Orion/Core/FileSystem/File.h"
#include "Orion/Core/FileSystem/PlatformFile.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ios>
#include <system_error>

#ifdef OE_PLATFORM_WINDOWS
    #include <cwctype>
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <Windows.h>
#endif

namespace Orion::PlatformFile
{
    namespace
    {
        // UTF-8 std::string <-> std::filesystem::path, without going through the Windows ANSI code page
        std::filesystem::path ToFsPath(const std::string& utf8)
        {
            return {std::u8string(utf8.begin(), utf8.end())};
        }

        std::string FromFsPath(const std::filesystem::path& path)
        {
            const std::u8string utf8 = path.generic_u8string(); // generic: always '/' separators
            return {utf8.begin(), utf8.end()};
        }

        class DesktopFile final : public IFile
        {
        public:
            DesktopFile(std::fstream stream, size_t size, FileMode mode)
                : m_Stream(std::move(stream)), m_Size(size), m_Mode(mode)
            {}

            [[nodiscard]] size_t Size() const override
            {
                return m_Size;
            }
            [[nodiscard]] size_t Tell() const override
            {
                return m_Position;
            }

            bool Seek(size_t offset) override
            {
                if (offset > m_Size)
                    return false;

                m_Stream.clear(); // a previous read may have hit EOF and left the stream in a failed state
                if (m_Mode == FileMode::Read)
                    m_Stream.seekg(static_cast<std::streamoff>(offset));
                else
                    m_Stream.seekp(static_cast<std::streamoff>(offset));

                m_Position = offset;
                return static_cast<bool>(m_Stream);
            }

            virtual size_t Read(void* buffer, size_t size) override
            {
                if (m_Mode != FileMode::Read)
                    return 0;

                m_Stream.read(static_cast<char*>(buffer), static_cast<std::streamsize>(size));
                const auto count = static_cast<size_t>(m_Stream.gcount());
                m_Position += count;
                return count;
            }

            virtual size_t Write(const void* data, size_t size) override
            {
                if (m_Mode != FileMode::Write)
                    return 0;

                m_Stream.write(static_cast<const char*>(data), static_cast<std::streamsize>(size));
                if (!m_Stream)
                    return 0;

                m_Position += size;
                m_Size = std::max(m_Size, m_Position);
                return size;
            }

            virtual void Flush() override
            {
                m_Stream.flush();
            }

        private:
            std::fstream m_Stream;
            size_t m_Size     = 0;
            size_t m_Position = 0;
            FileMode m_Mode;
        };

        // Windows resolves paths in a case-insensite way, so a incorrectly cased path works on Windows but fails on
        // other case-sensitive platforms
#if defined(OE_PLATFORM_WINDOWS) && defined(OE_DEBUG)
        std::wstring ToLower(std::wstring text)
        {
            std::ranges::transform(text, text.begin(), [](wchar_t c) {
                return static_cast<wchar_t>(std::towlower(c));
            });
            return text;
        }

        void WarnOnCaseMismatch(const std::filesystem::path& requested)
        {
            std::filesystem::path current = requested.root_path(); // Drive root (C:/) or empty for relative paths
            std::filesystem::path real    = current;
            bool mismatch                 = false;

            for (const std::filesystem::path& segment : requested.relative_path())
            {
                if (segment.empty())
                    continue; // trailing separator

                // Find the entry that matches ignoring case: that is the real file, even if miscased.
                // Segments not listed ("." and "..") keep their spelling.
                std::filesystem::path realSegment = segment;
                const std::wstring wanted         = ToLower(segment.wstring());
                std::error_code error;
                for (const auto& entry : std::filesystem::directory_iterator(current.empty() ? "." : current, error))
                {
                    const std::filesystem::path name = entry.path().filename();
                    if (ToLower(name.wstring()) == wanted)
                    {
                        realSegment = name;
                        break;
                    }
                }

                // Then compare case-sensitively
                mismatch = mismatch || realSegment.native() != segment.native();
                real /= realSegment;
                current /= segment; // Windows opens the miscased folder anyway
            }

            if (mismatch)
                OE_CORE_LOG(Warn,
                            "Path '{}' is '{}' on disk: it only works on case-insensitive file systems",
                            FromFsPath(requested),
                            FromFsPath(real));
        }
#endif

    } // namespace

    Scope<IFile> Open(const std::string& nativePath, FileMode mode)
    {
        const std::filesystem::path path = ToFsPath(nativePath);

        std::error_code error;
        std::fstream stream;
        size_t size = 0;

        if (mode == FileMode::Write)
        {
            if (path.has_parent_path())
                std::filesystem::create_directories(path.parent_path(), error);
            stream.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
        }
        else
        {
            size = static_cast<size_t>(std::filesystem::file_size(path, error));
            if (error)
                return nullptr;
#if defined(OE_PLATFORM_WINDOWS) && defined(OE_DEBUG)
            WarnOnCaseMismatch(path);
#endif
            stream.open(path, std::ios::in | std::ios::binary);
        }

        if (!stream.is_open())
            return nullptr;
        return CreateScope<DesktopFile>(std::move(stream), size, mode);
    }

    bool Exists(const std::string& nativePath)
    {
#if defined(OE_PLATFORM_WINDOWS) && defined(OE_DEBUG)
        const std::filesystem::path path = ToFsPath(nativePath);
        std::error_code error;
        const bool exists = std::filesystem::is_regular_file(path, error);
        if (exists)
            WarnOnCaseMismatch(path);
        return exists;
#else
        std::error_code error;
        return std::filesystem::is_regular_file(ToFsPath(nativePath), error);
#endif
    }

    std::string GetExecutableDirectory()
    {
#if defined(OE_PLATFORM_WINDOWS)
        std::wstring buffer(MAX_PATH, L'\0');
        DWORD length = 0;
        // GetModuleFileNameW returns the number of characters written in the buffer, excluding null terminator.
        // If the buffer is smaller than the path, it fills the buffer and truncates the path to its size.
        // Loop to increase the buffer size until the entire path fits inside (length returned by GetModuleFileNameW < buffer.size())
        while ((length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()))) ==
               buffer.size())
        {
            buffer.resize(buffer.size() * 2); // truncated: try again with a bigger buffer
        }
        if (length == 0)
            return "."; // GetModuleFileNameW failed, use the working directory
        // Resize to the real lenght size, drops the unused trailing '\0's
        buffer.resize(length);
        // parent_path to remove the executable name. The buffer is UTF-16, so we don't need to call ToFsPath
        return FromFsPath(std::filesystem::path(buffer).parent_path());
#elif defined(OE_PLATFORM_LINUX)
        std::error_code error;
        // Check where are we, and return the current working dir if it fails
        const std::filesystem::path executable = std::filesystem::read_symlink("/proc/self/exe", error);
        return error ? std::string(".") : FromFsPath(executable.parent_path());
#else
        OE_CORE_ASSERT(false, "GetExecutableDirectory is not implemented for this platform!");
        return ".";
#endif
    }

} // namespace Orion::PlatformFile
