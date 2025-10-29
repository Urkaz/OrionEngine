#pragma once

#include "Orion/Core.h"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#ifdef OE_ENABLE_LOGS

namespace OrionEngine
{
    class Log
    {
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return m_sCoreLogger;
        }
        static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return m_sClientLogger;
        }

    private:
        static std::shared_ptr<spdlog::logger> m_sCoreLogger;
        static std::shared_ptr<spdlog::logger> m_sClientLogger;
    };
} // namespace OrionEngine
#endif

enum OE_LogLevel
{
    trace,
    info,
    warn,
    error,
    fatal
};

#ifdef OE_ENABLE_LOGS
    #define OE_INTERNAL_LOGGER(OE_LogLevel, Logger, ...) OrionEngine::Log::Logger()->OE_LogLevel(__VA_ARGS__)
    #define OE_CORE_LOG(OE_LogLevel, ...) OE_INTERNAL_LOGGER(OE_LogLevel, GetCoreLogger, __VA_ARGS__)
    #define OE_LOG(OE_LogLevel, ...) OE_INTERNAL_LOGGER(OE_LogLevel, GetClientLogger, __VA_ARGS__)
#else
    #define OE_INTERNAL_LOGGER(OE_LogLevel, Logger, ...)
    #define OE_CORE_LOG(OE_LogLevel, ...)
    #define OE_LOG(OE_LogLevel, ...)
#endif
