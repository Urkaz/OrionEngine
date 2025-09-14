#pragma once

#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace ArcEngine
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        // Prevent copying
        WindowsWindow(const WindowsWindow&) = delete;
        WindowsWindow& operator=(const WindowsWindow&) = delete;

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) override {m_Data.EventCallback = callback; }
        virtual void SetVSync(bool enabled) override;
        virtual bool IsVSync() const override;

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    private:
        GLFWwindow* m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;

            WindowData(const std::string& title = "", unsigned int width = 0, unsigned int height = 0, bool vsync = false, const EventCallbackFn& eventCallback = EventCallbackFn())
                : Title(title), Width(width), Height(height), VSync(vsync), EventCallback(eventCallback)
            {
            }
        };

        WindowData m_Data;
    };

} // namespace ArcEngine
