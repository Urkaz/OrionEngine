#include "aepch.h"
#include "Application.h"

#include "Core/Log.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"

//#include <GLFW/glfw3.h>

namespace ArcEngine
{
    void Application::Run()
    {
        while (m_Running)
        {
            //glClearColor(1, 0, 1, 1);
            //glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
} // namespace ArcEngine
