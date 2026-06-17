#pragma once

#include "Orion/Core/Core.h"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#ifdef OE_ENABLE_LOGS

#include <utility>

namespace Orion
{
    class Log
    {
    public:
        static void Init();
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

    template <typename... Args>
    inline void LogMessage(Orion::LogLevel level, Ref<spdlog::logger>& logger, const std::string_view& fmt, Args&&... args)
    {
        switch (level)
        {
        case Orion::LogLevel::Trace:
            logger->trace(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        case Orion::LogLevel::Info:
            logger->info(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        case Orion::LogLevel::Warn:
            logger->warn(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        case Orion::LogLevel::Error:
            logger->error(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        case Orion::LogLevel::Critical:
            logger->critical(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        }
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
