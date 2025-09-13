#include "Application.h"

#include "Core/Log.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"

namespace ArcEngine
{
    void Application::Run()
    {
        WindowResizeEvent event(1280, 720);
        ArcEngine::Log::GetCoreLogger()->trace(event);

        while (true)
        {
        }
    }
} // namespace ArcEngine
