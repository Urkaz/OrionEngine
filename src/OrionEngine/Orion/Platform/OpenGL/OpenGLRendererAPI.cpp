#include "oepch.h"
#include "Orion/Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Orion
{
    void OpenGLMessageCallback([[maybe_unused]] unsigned source,
                               [[maybe_unused]] unsigned type,
                               [[maybe_unused]] unsigned id,
                               [[maybe_unused]] unsigned severity,
                               [[maybe_unused]] int length,
                               [[maybe_unused]] const char* message,
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

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // discard triangles facing away from the camera
        glFrontFace(GL_CCW); // counter-clockwise on screen = front face (OpenGL default)
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

    void OpenGLRendererAPI::SetDepthTest(bool enabled)
    {
        if (enabled)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetFaceCulling(bool enabled)
    {
        if (enabled)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        glDrawElements(GL_TRIANGLES,
                       static_cast<GLsizei>(vertexArray->GetIndexBuffer()->GetCount()),
                       GL_UNSIGNED_INT,
                       nullptr);
    }

    void OpenGLRendererAPI::DrawIndexed([[maybe_unused]] const Ref<VertexArray>& vertexArray,
                                        uint32_t indexCount,
                                        uint32_t firstIndex,
                                        uint32_t vertexOffset)
    {
        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 static_cast<GLsizei>(indexCount),
                                 GL_UNSIGNED_INT,
                                 reinterpret_cast<const void*>(static_cast<uintptr_t>(firstIndex) * sizeof(uint32_t)),
                                 static_cast<GLint>(vertexOffset)

        );
    }

} // namespace Orion
