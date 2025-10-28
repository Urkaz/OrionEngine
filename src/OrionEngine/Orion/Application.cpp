#include "oepch.h"
#include "Application.h"

#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/Event.h"
#include "Orion/Log.h"
#include "Orion/Input.h"

#include <glad/glad.h>

DISABLE_WARNING_FILE(4100, "-Wunused-parameter")

namespace OrionEngine
{
    Application* Application::s_Instance = nullptr;

    Application::Application() : m_Window(std::unique_ptr<Window>(Window::Create())), m_LayerStack()
    {
        OE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window->SetEventCallback(OE_BIND_EVENT_FN(Application::OnEvent));
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(OE_BIND_EVENT_FN(Application::OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
} // namespace OrionEngine
