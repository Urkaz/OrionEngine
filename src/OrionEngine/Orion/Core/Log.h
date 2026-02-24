#pragma once

#include "Orion/Core/Core.h"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#ifdef OE_ENABLE_LOGS

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
#endif


enum class OELogLevel
{
    Trace,
    Info,
    Warn,
    Error,
    Critical
};

#ifdef OE_ENABLE_LOGS
    #include <utility>
namespace Orion
{
    template <typename... Args>
    inline void LogMessage(OELogLevel level, Ref<spdlog::logger>& logger, const std::string_view& fmt, Args&&... args)
    {
        switch (level)
        {
        case OELogLevel::Trace:
            logger->trace(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        case OELogLevel::Info:
            logger->info(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        case OELogLevel::Warn:
            logger->warn(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        case OELogLevel::Error:
            logger->error(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        case OELogLevel::Critical:
            logger->critical(spdlog::fmt_lib::runtime(fmt), std::forward<Args>(args)...);
            break;
        }
    }
} // namespace Orion
#endif

#ifdef OE_ENABLE_LOGS
    #define OE_INTERNAL_LOGGER(Level, Logger, ...) Orion::LogMessage(Level, Orion::Log::Logger(), __VA_ARGS__)
    #define OE_CORE_LOG(Level, ...) OE_INTERNAL_LOGGER(OELogLevel::Level, GetCoreLogger, __VA_ARGS__)
    #define OE_LOG(Level, ...) OE_INTERNAL_LOGGER(OELogLevel::Level, GetClientLogger, __VA_ARGS__)
#else
    #define OE_INTERNAL_LOGGER(Level, Logger, ...)
    #define OE_CORE_LOG(Level, ...)
    #define OE_LOG(Level, ...)
#endif
