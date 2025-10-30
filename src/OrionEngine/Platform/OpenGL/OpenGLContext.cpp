#include "oepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/gl.h>

namespace OrionEngine
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

        OE_CORE_LOG(info, "OpenGL Info:");
        OE_CORE_LOG(info, "   Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        OE_CORE_LOG(info, "   Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        OE_CORE_LOG(info, "   Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        OE_CORE_LOG(info, "   Shading Language: {0}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }


} // namespace OrionEngine
