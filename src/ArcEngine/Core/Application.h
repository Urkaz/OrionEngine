#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace ArcEngine
{
    class ARCENGINE_API Application
    {
    public:
        Application()
            : m_Window(std::unique_ptr<Window>(Window::Create()))
        {
        }

        virtual ~Application() = default;

        void Run();

    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in client
    Application* CreateApplication();
} // namespace ArcEngine
