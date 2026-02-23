#include "oepch.h"
#include "Orion/Platform/OpenGL/OpenGLContext.h"

#include "Orion/Renderer/RendererAPI.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Orion
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
        OE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        OE_ASSERT(status, "Failed to initialize GLAD!");

        auto& capabilities    = RendererAPI::GetCapabilities();
        capabilities.Vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        capabilities.Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        capabilities.Version  = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        OE_CORE_LOG(info, "OpenGL Info:");
        OE_CORE_LOG(info, "   Vendor: {0}", capabilities.Vendor);
        OE_CORE_LOG(info, "   Renderer: {0}", capabilities.Renderer);
        OE_CORE_LOG(info, "   Version: {0}", capabilities.Version);

#ifdef OE_ENABLE_ASSERTS
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        OE_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
                       "Orion Engine requires at least OpenGL version 4.5!");
#endif
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }


} // namespace Orion
