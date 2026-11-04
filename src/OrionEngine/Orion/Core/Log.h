#pragma once

#include "Orion/Core/Core.h"

#include <spdlog/spdlog.h>

#ifdef OE_ENABLE_LOGS

namespace Orion
{
    class Log
    {
    public:
        static void Init(bool logToFile = true);
        static Ref<spdlog::logger>& GetCoreLogger()
        {
            return s_CoreLogger;
        }
        static Ref<spdlog::logger>& GetClientLogger()
        {
            return s_ClientLogger;
        }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };
} // namespace Orion

namespace Orion
{

    enum class LogLevel
    {
        Trace,
        Info,
        Warn,
        Error,
        Critical
    };

    constexpr spdlog::level::level_enum ToSpdlogLevel(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Trace:
            return spdlog::level::trace;
        case LogLevel::Info:
            return spdlog::level::info;
        case LogLevel::Warn:
            return spdlog::level::warn;
        case LogLevel::Error:
            return spdlog::level::err;
        case LogLevel::Critical:
            return spdlog::level::critical;
        }
        return spdlog::level::off;
    }

    #if defined(OE_ENABLE_PROFILING)
    constexpr uint32_t LogLevelColor(Orion::LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Trace:
            return 0x999999;
        case LogLevel::Info:
            return 0xFFFFFF;
        case LogLevel::Warn:
            return 0xFFAA00;
        case LogLevel::Error:
            return 0xFF3333;
        case LogLevel::Critical:
            return 0xFF0000;
        }
        return 0xFFFFFF;
    }
    #endif

    void WriteLog(Orion::LogLevel level,
                  spdlog::logger& logger,
                  spdlog::string_view_t fmt,
                  spdlog::fmt_lib::format_args args);


    template <typename... Args>
    inline void LogMessage(Orion::LogLevel level,
                           Ref<spdlog::logger>& logger,
                           spdlog::format_string_t<Args...> fmt,
                           Args&&... args)
    {
        if (!logger->should_log(ToSpdlogLevel(level)))
            return;

        WriteLog(level, *logger, fmt.get(), spdlog::fmt_lib::make_format_args(args...));
    }
} // namespace Orion
#endif

#ifdef OE_ENABLE_LOGS
    #define OE_INTERNAL_LOGGER(Level, Logger, ...) Orion::LogMessage(Level, Orion::Log::Logger(), __VA_ARGS__)
    #define OE_CORE_LOG(Level, ...) OE_INTERNAL_LOGGER(Orion::LogLevel::Level, GetCoreLogger, __VA_ARGS__)
    #define OE_LOG(Level, ...) OE_INTERNAL_LOGGER(Orion::LogLevel::Level, GetClientLogger, __VA_ARGS__)
#else
    #define OE_INTERNAL_LOGGER(Level, Logger, ...)
    #define OE_CORE_LOG(Level, ...)
    #define OE_LOG(Level, ...)
#endif
