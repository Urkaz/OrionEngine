#include "oepch.h"

#include "Orion/Core/FileSystem/File.h"
#include "Orion/Core/FileSystem/FileSystem.h"
#include "Orion/Core/Log.h"
#include "Orion/Profiling/Profiling.h"

#include <spdlog/common.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef OE_ENABLE_LOGS
namespace Orion
{
    namespace
    {
        class FileSystemSink final : public spdlog::sinks::base_sink<std::mutex>
        {
        public:
            explicit FileSystemSink(std::string_view virtualPath)
                : m_File(FileSystem::Open(virtualPath, FileMode::Write))
            {}

            virtual void sink_it_(const spdlog::details::log_msg& msg) override
            {
                if (!m_File)
                    return;

                spdlog::memory_buf_t formatted;
                formatter_->format(msg, formatted);
                m_File->Write(formatted.data(), formatted.size());
            }
            virtual void flush_() override
            {
                if (m_File)
                    m_File->Flush();
            }

        private:
            Scope<IFile> m_File;
        };
    } // namespace

    Ref<spdlog::logger> Log::s_CoreLogger;
    Ref<spdlog::logger> Log::s_ClientLogger;

    void Log::Init(bool logToFile)
    {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks[0]->set_pattern("%^[%T] %n: %v%$");

        if (logToFile)
        {
            logSinks.emplace_back(std::make_shared<FileSystemSink>("user://Orion.log"));
            logSinks[1]->set_pattern("[%T] [%l] %n: %v");
        }

        s_CoreLogger = std::make_shared<spdlog::logger>("OrionEngine", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_CoreLogger);
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::warn);

        s_ClientLogger = std::make_shared<spdlog::logger>("App", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::warn);

        spdlog::flush_every(std::chrono::seconds(2)); // background thread flushes the registered loggers

        OE_CORE_LOG(Trace, "Initialized Core Log!");
        OE_LOG(Trace, "Initialized Client Log!");
    }

    void Log::Shutdown()
    {
        spdlog::shutdown();
        s_ClientLogger.reset();
        s_CoreLogger.reset(); // The sink and its IFile die, which flushes and closes the file
    }

    void WriteLog(LogLevel level, spdlog::logger& logger, spdlog::string_view_t fmt, spdlog::fmt_lib::format_args args)
    {
        std::string message = spdlog::fmt_lib::vformat(fmt, args);
        logger.log(ToSpdlogLevel(level), message);
        OE_PROFILE_LOG_MESSAGE(message, LogLevelColor(level));
    }
} // namespace Orion
#endif
