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
// Función inline para mapear OELogLevel a la función de spdlog
#include <utility>
namespace Orion {
    template<typename... Args>
    inline void LogMessage(OELogLevel level, Ref<spdlog::logger>& logger, fmt::format_string<Args...> fmt, Args&&... args)
    {
        switch (level)
        {
        case OELogLevel::Trace:
            logger->trace(fmt, std::forward<Args>(args)...);
            break;
        case OELogLevel::Info:
            logger->info(fmt, std::forward<Args>(args)...);
            break;
        case OELogLevel::Warn:
            logger->warn(fmt, std::forward<Args>(args)...);
            break;
        case OELogLevel::Error:
            logger->error(fmt, std::forward<Args>(args)...);
            break;
        case OELogLevel::Critical:
            logger->critical(fmt, std::forward<Args>(args)...);
            break;
        }
    }
}
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
