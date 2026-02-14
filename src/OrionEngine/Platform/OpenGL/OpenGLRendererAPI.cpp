#include "oepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

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
