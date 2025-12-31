#pragma once

#include "Orion/Core.h"
#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/Event.h"
#include "Orion/ImGui/ImGuiLayer.h"
#include "Orion/LayerStack.h"
#include "Orion/Window.h"

#include "Orion/Renderer/Buffer.h"
#include "Orion/Renderer/Shader.h"
#include "Orion/Renderer/VertexArray.h"

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

        static Application* s_Instance;

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;
        bool m_Running = true;

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;

        std::shared_ptr<Shader> m_BlueShader;
        std::shared_ptr<VertexArray> m_SquareVA;
    };

    // To be defined in client
    Application* CreateApplication();
} // namespace Orion
