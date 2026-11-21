#include "Orion/Platform/OpenGL/OpenGLContext.h"

#include "Orion/Core/Base.h"
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
        OE_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        const GLADloadproc loader = [](const char* name) -> void* {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast): GLAD wants the function pointer as void*
            return reinterpret_cast<void*>(glfwGetProcAddress(name));
        };
        [[maybe_unused]] const int status = gladLoadGLLoader(loader);
        OE_ASSERT(status, "Failed to initialize GLAD!");

        auto& capabilities = RendererAPI::GetCapabilities();
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast): glGetString returns the text as const GLubyte*
        capabilities.Vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        capabilities.Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        capabilities.Version  = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

        OE_CORE_LOG(Info, "OpenGL Info:");
        OE_CORE_LOG(Info, "   Vendor: {0}", capabilities.Vendor);
        OE_CORE_LOG(Info, "   Renderer: {0}", capabilities.Renderer);
        OE_CORE_LOG(Info, "   Version: {0}", capabilities.Version);

#ifdef OE_ENABLE_ASSERTS
        int versionMajor = 0;
        int versionMinor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        OE_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
                       "Orion Engine requires at least OpenGL version 4.5!");
#endif
    }

    void OpenGLContext::SwapBuffers()
    {
        OE_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }


} // namespace Orion
