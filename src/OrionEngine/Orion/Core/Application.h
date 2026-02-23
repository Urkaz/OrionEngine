#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/Events/ApplicationEvent.h"
#include "Orion/Core/Events/Event.h"
#include "Orion/Core/LayerStack.h"
#include "Orion/Core/Timestep.h"
#include "Orion/Core/Window.h"
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
        virtual ~Application();

        /**
         * @brief Main application loop.
         *
         * Processes events, updates layers, and renders until m_Running is false.
         * Calculates delta time between frames for frame-rate independent updates.
         */
        void Run();

        /**
         * @brief Dispatch an event through the layer stack.
         *
         * Propagates events to layers in reverse order (top to bottom)
         * and handles application-level events like WindowCloseEvent.
         *
         * @param e The event to process.
         */
        void OnEvent(Event& e);

        /**
         * @brief Add a layer to the layer stack.
         *
         * @param layer Pointer to the layer to add (inserted before overlays).
         */
        void PushLayer(Layer* layer);

        /**
         * @brief Add an overlay layer.
         *
         * @param overlay Pointer to the overlay layer (inserted at the top, rendered last).
         */
        void PushOverlay(Layer* overlay);

        /**
         * @brief Get the singleton instance of the Application.
         *
         * @return Application& Reference to the running application instance.
         */
        static Application& Get()
        {
            return *s_Instance;
        }

        /**
         * @brief Get the application window.
         *
         * @return Window& Reference to the window instance.
         */
        inline Window& GetWindow()
        {
            return *m_Window;
        }

    private:
        /**
         * @brief Handle window close event.
         *
         * @param e The window close event.
         * @return true if handled, false otherwise.
         */
        bool OnWindowClose(WindowCloseEvent& e);

        bool OnWindowResize(WindowResizeEvent& e);

    private:
        static Application* s_Instance; ///< Singleton instance pointer

        Scope<Window> m_Window;   ///< Main application window
        ImGuiLayer* m_ImGuiLayer; ///< ImGui overlay layer
        LayerStack m_LayerStack;  ///< Stack of application layers
        bool m_Running   = true;  ///< Application running state
        bool m_Minimized = false; ///< Application minimized state

        Timestep m_Timestep;          ///< Delta time for current frame
        float m_LastFrameTime = 0.0f; ///< Timestamp of last frame
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
