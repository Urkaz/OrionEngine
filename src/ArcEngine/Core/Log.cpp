#include "aepch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef AE_ENABLE_LOGS
namespace ArcEngine
{
    std::shared_ptr<spdlog::logger> Log::m_sCoreLogger;
    std::shared_ptr<spdlog::logger> Log::m_sClientLogger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        m_sCoreLogger = spdlog::stdout_color_mt("ArcEngine");
        m_sCoreLogger->set_level(spdlog::level::trace);

        m_sClientLogger = spdlog::stdout_color_mt("App");
        m_sClientLogger->set_level(spdlog::level::trace);

        AE_CORE_LOG(warn, "Initialized Core Log!");
        AE_LOG(info, "Initialized Client Log!");
    }
} // namespace ArcEngine
#endif
