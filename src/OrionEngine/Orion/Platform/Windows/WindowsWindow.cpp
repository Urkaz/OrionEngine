#include "oepch.h"
#include "Orion/Platform/Windows/WindowsWindow.h"

#include "Orion/Core/Events/ApplicationEvent.h"
#include "Orion/Core/Events/KeyEvent.h"
#include "Orion/Core/Events/MouseEvent.h"
#include "Orion/Platform/OpenGL/OpenGLContext.h"
#include "Orion/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Orion
{
    static uint8_t s_GLFWWindowCount = 0;

    namespace
    {
        void GLFWErrorCallback([[maybe_unused]] int errorCode, [[maybe_unused]] const char* description)
        {
            OE_CORE_LOG(Error, "GLFW Error ({0}): {1}", errorCode, description);
        }
    } // namespace

    WindowsWindow::WindowsWindow(const WindowProps& props) : m_Window(nullptr), m_Context(nullptr), m_Data()
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

        OE_CORE_LOG(Info, "Creating window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);

        if (s_GLFWWindowCount == 0)
        {
            OE_CORE_LOG(Info, "Initializing GLFW");
            [[maybe_unused]] const int success = glfwInit();
            OE_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

#if defined(OE_DEBUG)
        if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        m_Window = glfwCreateWindow(static_cast<int>(props.Width),
                                    static_cast<int>(props.Height),
                                    m_Data.Title.c_str(),
                                    nullptr,
                                    nullptr);
        ++s_GLFWWindowCount;

        m_Context = GraphicsContext::Create(m_Window);
        m_Context->Init();

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

        glfwSetKeyCallback(
            m_Window,
            [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
                const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(static_cast<KeyCode>(key));
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                    data.EventCallback(event);
                    break;
                }
                default:
                    break;
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(static_cast<KeyCode>(keycode));
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
            const WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
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
        --s_GLFWWindowCount;
        if (s_GLFWWindowCount == 0)
        {
            OE_CORE_LOG(Info, "Terminating GLFW");
            glfwTerminate();
        }
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
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

} // namespace Orion
