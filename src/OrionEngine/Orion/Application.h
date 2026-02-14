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
    /**
     * @class Application
     * @brief Main application class that manages the engine lifecycle.
     *
     * Handles window creation, layer management, event dispatching, and
     * the main update/render loop. The application runs until m_Running
     * is set to false (typically by a window close event).
     */
    class Application
    {
    public:
        /**
         * @brief Constructor for the Application.
         *
         * Initializes the window, layer stack, and sets up the ImGui layer.
         */
        Application();

        /**
         * @brief Virtual destructor for proper cleanup.
         */
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

    /**
     * @brief Factory function to create the client application.
     *
     * This function must be defined by the client application.
     * It creates and returns a new Application-derived instance.
     *
     * @return Application* Pointer to a dynamically allocated Application instance.
     */
    Application* CreateApplication();
} // namespace Orion
