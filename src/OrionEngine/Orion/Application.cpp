#include "oepch.h"
#include "Application.h"

#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/Event.h"
#include "Orion/Input.h"
#include "Orion/Log.h"

#include <glfw/glfw3.h>

#include <cstdint>

DISABLE_WARNING_FILE(4100, "-Wunused-parameter")

namespace Orion
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
        : m_Window(std::unique_ptr<Window>(Window::Create())), m_ImGuiLayer(new ImGuiLayer()), m_LayerStack()
    {
        OE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window->SetEventCallback(OE_BIND_EVENT_FN(Application::OnEvent));

        PushOverlay(m_ImGuiLayer);
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time        = static_cast<float>(glfwGetTime()); // Platform::GetTime
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime   = time;

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImguiRender();
            m_ImGuiLayer->End();

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
} // namespace Orion
