#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/Window.h"
#include "Orion/Renderer/GraphicsContext.h"

#include <utility>

struct GLFWwindow;

namespace Orion
{
    class GlfwWindow final : public Window
    {
    public:
        explicit GlfwWindow(const WindowProps& props);
        ~GlfwWindow() override;
        OE_NON_COPYABLE(GlfwWindow);

        void OnUpdate() override;

        [[nodiscard]] inline unsigned int GetWidth() const override
        {
            return m_Data.Width;
        }
        [[nodiscard]] inline unsigned int GetHeight() const override
        {
            return m_Data.Height;
        }

        // Window attributes
        void SetEventCallback(const EventCallbackFn& callback) override
        {
            m_Data.EventCallback = callback;
        }
        void SetVSync(bool enabled) override;
        [[nodiscard]] bool IsVSync() const override;

        [[nodiscard]] inline void* GetNativeWindow() const override
        {
            return m_Window;
        }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        GLFWwindow* m_Window{nullptr};
        UniquePtr<GraphicsContext> m_Context;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;

            explicit WindowData(std::string title    = "",
                       unsigned int width            = 0,
                       unsigned int height           = 0,
                       bool vsync                    = false,
                       EventCallbackFn eventCallback = EventCallbackFn())
                : Title(std::move(title)), Width(width), Height(height), VSync(vsync),
                  EventCallback(std::move(eventCallback))
            {}
        };

        WindowData m_Data;
    };

} // namespace Orion
