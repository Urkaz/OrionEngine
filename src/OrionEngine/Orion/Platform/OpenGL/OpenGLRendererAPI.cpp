#include "oepch.h"
#include "Orion/Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Orion
{
    void OpenGLMessageCallback([[maybe_unused]] unsigned source,
                               [[maybe_unused]] unsigned type,
                               [[maybe_unused]] unsigned id,
                               unsigned severity,
                               [[maybe_unused]] int length,
                               const char* message,
                               [[maybe_unused]] const void* userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            OE_CORE_LOG(Critical, message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            OE_CORE_LOG(Error, message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            OE_CORE_LOG(Warn, message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            OE_CORE_LOG(Trace, message);
            return;
        }
        OE_CORE_ASSERT(false, "Unknown severity level!");
    }

    void OpenGLRendererAPI::Init()
    {
        OE_PROFILE_FUNCTION();


#ifdef OE_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLint>(width), static_cast<GLint>(height));
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        glDrawElements(GL_TRIANGLES,
                       static_cast<int>(vertexArray->GetIndexBuffer()->GetCount()),
                       GL_UNSIGNED_INT,
                       nullptr);
    }

} // namespace Orion
