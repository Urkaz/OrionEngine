#include "oepch.h"
#include "Orion/Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

void Orion::OpenGLRendererAPI::Init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void Orion::OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLint>(width), static_cast<GLint>(height));
}

void Orion::OpenGLRendererAPI::SetClearColor(const glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Orion::OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Orion::OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
{
    glDrawElements(GL_TRIANGLES, static_cast<int>(vertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);
}
