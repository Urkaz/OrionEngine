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

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawIndexed(vertexArray);
        }

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
} // namespace Orion
