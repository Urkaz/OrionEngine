#pragma once

#include "Orion/Renderer/RendererAPI.h"

namespace Orion
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;

        virtual void SetClearColor(const glm::vec4 color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    };

} // namespace Orion
