#pragma once

#include "Core.h"
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#ifdef AE_ENABLE_LOGS
// #if AE_PLATFORM_WINDOWS
// #pragma warning(disable: 4251)
// #endif
namespace ArcEngine
{
    class ARCENGINE_API Log
    {
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_sCoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_sClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> m_sCoreLogger;
        static std::shared_ptr<spdlog::logger> m_sClientLogger;
    };
} // namespace ArcEngine
#endif

enum AE_LogLevel
{
    trace,
    info,
    warn,
    error,
    fatal
};

#ifdef AE_ENABLE_LOGS
    #define AE_INTERNAL_LOGGER(AE_LogLevel, Logger, ...) ArcEngine::Log::Logger()->AE_LogLevel(__VA_ARGS__)
    #define AE_CORE_LOG(AE_LogLevel, ...) AE_INTERNAL_LOGGER(AE_LogLevel, GetCoreLogger, __VA_ARGS__)
    #define AE_LOG(AE_LogLevel, ...) AE_INTERNAL_LOGGER(AE_LogLevel, GetClientLogger, __VA_ARGS__)
#else
    #define AE_INTERNAL_LOGGER(AE_LogLevel, Logger, ...)
    #define AE_CORE_LOG(AE_LogLevel, ...)
    #define AE_LOG(AE_LogLevel, ...)
#endif
