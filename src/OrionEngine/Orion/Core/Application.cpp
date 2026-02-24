#include "oepch.h"
#include "Orion/Core/Application.h"

#include "Orion/Core/Events/ApplicationEvent.h"
#include "Orion/Core/Events/Event.h"
#include "Orion/Core/Input.h"
#include "Orion/Core/Log.h"
#include "Orion/Core/Window.h"
#include "Orion/Renderer/Renderer.h"

#include <cstdint>
#include <glfw/glfw3.h>

namespace Orion
{
    Application* Application::s_Instance = nullptr;

    Application::Application() : m_Window(Window::Create()), m_ImGuiLayer(new ImGuiLayer()), m_LayerStack()
    {
        OE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window->SetEventCallback(OE_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        Renderer::Shutdown();
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time        = static_cast<float>(glfwGetTime()); // Platform::GetTime
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime   = time;

            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timestep);
            }

            Application* app = this;
            Renderer::Submit([app]() { app->RenderImGui(); });

            Renderer::Get().WaitAndRender();

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

    void Application::RenderImGui()
    {
        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(OE_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(OE_BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            (*it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    bool Application::OnWindowClose([[maybe_unused]] WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;

        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
} // namespace Orion
