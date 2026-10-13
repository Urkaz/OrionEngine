#pragma once

#include "Orion/Renderer/Renderer.h"
#include "Orion/Renderer/RendererAPI.h"

namespace Orion
{
    class RenderCommand
    {
    public:
        inline static void Init()
        {
            Renderer::Submit([]() { s_RendererAPI->Init(); });
        }

        inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            Renderer::Submit([x, y, width, height]() { s_RendererAPI->SetViewPort(x, y, width, height); });
        }

        inline static void SetClearColor(const glm::vec4 color)
        {
            Renderer::Submit([color]() { s_RendererAPI->SetClearColor(color); });
        }

        inline static void Clear()
        {
            Renderer::Submit([]() { s_RendererAPI->Clear(); });
        }

        inline static void SetDepthTest(bool enabled)
        {
            Renderer::Submit([enabled]() { s_RendererAPI->SetDepthTest(enabled); });
        }

        inline static void SetFaceCulling(bool enabled)
        {
            Renderer::Submit([enabled]() { s_RendererAPI->SetFaceCulling(enabled); });
        }

        // RT_ = render thread: only valid inside a command already submitted with Renderer::Submit
        inline static void RT_DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            OE_CORE_ASSERT(Renderer::IsExecutingCommands(), "RT_ function called outside a render command");
            s_RendererAPI->DrawIndexed(vertexArray);
        }

        inline static void RT_DrawIndexed(const Ref<VertexArray>& vertexArray,
                                          uint32_t indexCount,
                                          uint32_t firstIndex,
                                          uint32_t vertexOffset)
        {
            OE_CORE_ASSERT(Renderer::IsExecutingCommands(), "RT_ function called outside a render command");
            s_RendererAPI->DrawIndexed(vertexArray, indexCount, firstIndex, vertexOffset);
        }

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
} // namespace Orion
