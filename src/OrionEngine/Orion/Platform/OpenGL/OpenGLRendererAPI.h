#pragma once

#include "Orion/Renderer/RendererAPI.h"

namespace Orion
{
    class OpenGLRendererAPI final : public RendererAPI
    {
    public:
        void Init() override;
        void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4 color) override;
        void Clear() override;
        void SetDepthTest(bool enabled) override;
        void SetFaceCulling(bool enabled) override;

        void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
        void DrawIndexed(const Ref<VertexArray>& vertexArray,
                         uint32_t indexCount,
                         uint32_t firstIndex,
                         uint32_t vertexOffset) override;
    };

} // namespace Orion
