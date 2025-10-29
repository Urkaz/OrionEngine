#pragma once

#include "Orion/Core.h"
#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/Event.h"
#include "Orion/ImGui/ImGuiLayer.h"
#include "Orion/LayerStack.h"
#include "Orion/Window.h"

namespace OrionEngine
{
    class Application
    {
    public:
        Application();

        virtual ~Application() = default;

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        static Application& Get()
        {
            return *s_Instance;
        }

        inline Window& GetWindow()
        {
            return *m_Window;
        }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        static Application* s_Instance;

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;
        bool m_Running = true;
    };

    // To be defined in client
    Application* CreateApplication();
} // namespace OrionEngine
