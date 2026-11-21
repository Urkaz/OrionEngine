#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/Events/Event.h"

#include <functional>
#include <utility>

namespace Orion
{
    /**
     * @struct WindowProps
     * @brief Configuration structure for the application window.
     *
     * Contains the parameters necessary to create and initialize a window,
     * including title and dimensions.
     */
    struct WindowProps
    {
        std::string Title;   ///< Window title
        unsigned int Width;  ///< Window width in pixels
        unsigned int Height; ///< Window height in pixels

        /**
         * @brief Constructor for WindowProps with default values.
         *
         * @param title Window title (default "Orion Engine")
         * @param width Window width in pixels (default 1280)
         * @param height Window height in pixels (default 720)
         */
        explicit WindowProps(std::string title = "Orion Engine", unsigned int width = 1280, unsigned int height = 720)
            : Title(std::move(title)), Width(width), Height(height)
        {}
    };

    /**
     * @class Window
     * @brief Abstract base class for platform-specific window implementation.
     *
     * Defines the interface for creating and managing application windows.
     * Concrete implementations must be provided for specific platforms (e.g., GLFW).
     */
    class Window
    {
    public:
        /**
         * @typedef EventCallbackFn
         * @brief Function signature for event callback handlers.
         *
         * Callback function that receives events triggered by the window.
         */
        using EventCallbackFn = std::function<void(Event&)>;

        Window()          = default;
        virtual ~Window() = default;
        OE_NON_COPYABLE(Window);

        /**
         * @brief Updates the window state and processes pending events.
         *
         * Should be called each frame to handle window updates and event processing.
         */
        virtual void OnUpdate() = 0;

        /**
         * @brief Retrieves the width of the window.
         *
         * @return unsigned int The window width in pixels.
         */
        [[nodiscard]] virtual unsigned int GetWidth() const = 0;

        /**
         * @brief Retrieves the height of the window.
         *
         * @return unsigned int The window height in pixels.
         */
        [[nodiscard]] virtual unsigned int GetHeight() const = 0;

        /**
         * @brief Sets the event callback function for the window.
         *
         * @param callback The function to be called when events occur.
         */
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        /**
         * @brief Enables or disables vertical synchronization (VSync).
         *
         * @param enabled True to enable VSync, false to disable.
         */
        virtual void SetVSync(bool enabled) = 0;

        /**
         * @brief Checks if vertical synchronization is enabled.
         *
         * @return bool True if VSync is enabled, false otherwise.
         */
        [[nodiscard]] virtual bool IsVSync() const = 0;

        /**
         * @brief Retrieves the native window pointer.
         *
         * Returns a platform-specific pointer to the underlying window object.
         * The exact type depends on the platform (e.g., GLFWwindow*).
         *
         * @return void* Pointer to the native window object.
         */
        [[nodiscard]] virtual void* GetNativeWindow() const = 0;

        /**
         * @brief Factory method to create platform-specific window implementation.
         *
         * @param props Configuration properties for the window.
         * @return UniquePtr<Window> Owning pointer to the newly created Window instance.
         */
        [[nodiscard]] static UniquePtr<Window> Create(const WindowProps& props = WindowProps());
    };

} // namespace Orion
