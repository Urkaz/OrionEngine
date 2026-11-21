#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/FileSystem/File.h"

#include <mutex>
#include <spdlog/details/log_msg.h>
#include <spdlog/sinks/base_sink.h>
#include <string_view>

namespace Orion::Internal
{
    // spdlog sink that writes through the VFS, so the log file can live in user://
    class FileSystemSink final : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        explicit FileSystemSink(std::string_view virtualPath);

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override;
        void flush_() override;

    private:
        UniquePtr<IFile> m_File;
    };
} // namespace Orion::Internal
