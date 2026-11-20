#include "Orion/Core/Internal/FileSystemSink.h"

#include "Orion/Core/FileSystem/FileSystem.h"

#include <spdlog/common.h>

namespace Orion::Internal
{
    FileSystemSink::FileSystemSink(std::string_view virtualPath)
        : m_File(FileSystem::Open(virtualPath, FileMode::Write))
    {}

    void FileSystemSink::sink_it_(const spdlog::details::log_msg& msg)
    {
        if (!m_File)
            return;

        spdlog::memory_buf_t formatted;
        formatter_->format(msg, formatted);
        m_File->Write(formatted.data(), formatted.size());
    }

    void FileSystemSink::flush_()
    {
        if (m_File)
            m_File->Flush();
    }
} // namespace Orion::Internal
