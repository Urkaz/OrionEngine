#include "oepch.h"

#include "Orion/Core/Log.h"
#include "Orion/Profiling/Profiling.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef OE_ENABLE_LOGS
namespace Orion
{
    Ref<spdlog::logger> Log::s_CoreLogger;
    Ref<spdlog::logger> Log::s_ClientLogger;

    void Log::Init(bool logToFile)
    {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks[0]->set_pattern("%^[%T] %n: %v%$");

        if (logToFile)
        {
            logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Orion.log", true));
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


        OE_CORE_LOG(Trace, "Initialized Core Log!");
        OE_LOG(Trace, "Initialized Client Log!");
    }

    void WriteLog(LogLevel level, spdlog::logger& logger, spdlog::string_view_t fmt, spdlog::fmt_lib::format_args args)
    {
        std::string message = spdlog::fmt_lib::vformat(fmt, args);
        logger.log(ToSpdlogLevel(level), message);
        OE_PROFILE_LOG_MESSAGE(message, LogLevelColor(level));
    }
} // namespace Orion
#endif
