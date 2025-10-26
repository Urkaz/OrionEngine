#include "oepch.h"

#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"
#include "WindowsWindow.h"

namespace OrionEngine
{
    static bool s_GLFWInitialized = false;

    namespace
    {
        void GLFWErrorCallback(int error, const char* description)
        {
            OE_CORE_LOG(error, "GLFW Error ({0}): {1}", error, description);
        }
    }

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props) : m_Window(nullptr), m_Data()
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title  = props.Title;
        m_Data.Width  = props.Width;
        m_Data.Height = props.Height;

        OE_CORE_LOG(info, "Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            // TODO: glfwTerminate on system shutdown
            const int success = glfwInit();
            OE_CORE_ASSERT(success, "Could not initialize GLFW!");

            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(static_cast<int>(props.Width),
                                    static_cast<int>(props.Height),
                                    m_Data.Title.c_str(),
                                    nullptr,
                                    nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.Width       = static_cast<unsigned int>(width);
            data.Height      = static_cast<unsigned int>(height);

            WindowResizeEvent event(data.Width, data.Height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
            default:
                break;
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
            default:
                break;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            data.EventCallback(event);
        });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

} // namespace OrionEngine
