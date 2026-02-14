#pragma once

#include "Orion/Core.h"

#include "Orion/Core/Timestep.h"
#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/Event.h"
#include "Orion/LayerStack.h"
#include "Orion/Window.h"

#include "Orion/ImGui/ImGuiLayer.h"

namespace Orion
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

    private:
        static Application* s_Instance;

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;
        bool m_Running = true;

        Timestep m_Timestep;
        float m_LastFrameTime = 0.0f;
    };

    // To be defined in client
    Application* CreateApplication();
} // namespace Orion
