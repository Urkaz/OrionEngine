#include "oepch.h"

#include "Orion/Renderer/Renderer.h"

#include "Orion/Renderer/LightingMath.h"
#include "Orion/Renderer/MeshRenderData.h"
#include "Orion/Renderer/MeshSource.h"
#include "Orion/Renderer/RenderCommand.h"
#include "Orion/Renderer/Renderer.h"
#include "Orion/Renderer/Renderer2D.h"

#include <cstdint>

namespace Orion
{
    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

    static Scope<RenderCommandQueue> s_CommandQueue = CreateScope<RenderCommandQueue>();

    void Renderer::Init()
    {
        OE_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        OE_PROFILE_FUNCTION();

        // Run (and destroy) whatever is still queued while every system and the GL context are alive.
        // Must happen BEFORE Renderer2D::Shutdown: pending commands may use its data.
        WaitAndRender();

        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewPort(0, 0, width, height);
    }

    void Renderer::BeginScene(const Camera& camera, const glm::mat4& viewMatrix)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetProjectionMatrix() * viewMatrix;
    }

    void Renderer::EndScene() {}

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
    {
        const glm::mat4 viewProjection = s_SceneData->ViewProjectionMatrix;
        Renderer::Submit([shader, vertexArray, transform, viewProjection]() {
            shader->Bind();
            shader->SetMat4("u_ViewProjection", viewProjection);
            shader->SetMat4("u_Transform", transform);

            vertexArray->Bind();
            RenderCommand::RT_DrawIndexed(vertexArray);
        });
    }

    void Renderer::SubmitMesh(const Ref<Shader>& shader,
                              const Ref<MeshRenderData>& renderData,
                              const glm::mat4& transform)
    {
        const MeshSource& source           = *renderData->GetSource();
        const Ref<VertexArray> vertexArray = renderData->GetVertexArray();
        const glm::mat4 viewProjection     = s_SceneData->ViewProjectionMatrix;

        for (const MeshNode& node : source.Nodes)
        {
            const glm::mat4 world = transform * node.WorldTransform;

            for (const uint32_t submeshIndex : node.Submeshes)
            {
                const Submesh& submesh = source.Submeshes[submeshIndex];
                if (submesh.IndexCount == 0)
                    continue;

                const MeshRenderMaterial& material = renderData->GetMaterial(submesh.MaterialIndex);

                Renderer::Submit([shader,
                                  vertexArray,
                                  viewProjection,
                                  world,
                                  normalMatrix = NormalMatrix(world),
                                  albedoTex    = material.Albedo,
                                  normalTex    = material.Normal,
                                  hasNormalMap = material.HasNormalMap,
                                  submeshIndex,
                                  indexCount   = submesh.IndexCount,
                                  firstIndex   = submesh.FirstIndex,
                                  vertexOffset = submesh.VertexOffset]() {
                    shader->Bind();
                    shader->SetMat4("u_ViewProjection", viewProjection);
                    shader->SetMat4("u_Transform", world);
                    shader->SetMat3("u_NormalMatrix", normalMatrix);
                    shader->SetInt("u_SubmeshIndex", static_cast<int>(submeshIndex));

                    albedoTex->Bind(0);
                    normalTex->Bind(1);
                    shader->SetInt("u_Albedo", 0);
                    shader->SetInt("u_NormalMap", 1);
                    shader->SetInt("u_HasNormalMap", hasNormalMap ? 1 : 0);

                    vertexArray->Bind();
                    RenderCommand::RT_DrawIndexed(vertexArray, indexCount, firstIndex, vertexOffset);
                });
            }
        }
    }

    RenderCommandQueue& Renderer::GetRenderCommandQueue()
    {
        return *s_CommandQueue;
    }

} // namespace Orion
