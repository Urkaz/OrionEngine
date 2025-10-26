#include "oepch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef OE_ENABLE_LOGS
namespace OrionEngine
{
    std::shared_ptr<spdlog::logger> Log::m_sCoreLogger;
    std::shared_ptr<spdlog::logger> Log::m_sClientLogger;

    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        m_sCoreLogger = spdlog::stdout_color_mt("OrionEngine");
        m_sCoreLogger->set_level(spdlog::level::trace);

        m_sClientLogger = spdlog::stdout_color_mt("App");
        m_sClientLogger->set_level(spdlog::level::trace);

        OE_CORE_LOG(warn, "Initialized Core Log!");
        OE_LOG(info, "Initialized Client Log!");
    }
} // namespace OrionEngine
#endif
