#include "oepch.h"
#include "Renderer.h"

#include "RenderCommand.h"

namespace Orion
{
    Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {}

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4 transform)
    {
        shader->Bind();
        shader->UploadeUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->UploadeUniformMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

} // namespace Orion
