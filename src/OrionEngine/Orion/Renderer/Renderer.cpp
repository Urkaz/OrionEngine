#include "oepch.h"
#include "Orion/Renderer/Renderer.h"

#include "Orion/Renderer/RenderCommand.h"
#include "Orion/Renderer/Renderer2D.h"
#include "Renderer.h"

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

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {}

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
    {
        Renderer::Submit([shader, vertexArray, transform]() {
            shader->Bind();
            shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
            shader->SetMat4("u_Transform", transform);

            vertexArray->Bind();
            RenderCommand::RT_DrawIndexed(vertexArray);
        });
    }

    RenderCommandQueue& Renderer::GetRenderCommandQueue()
    {
        return *s_CommandQueue;
    }

} // namespace Orion
