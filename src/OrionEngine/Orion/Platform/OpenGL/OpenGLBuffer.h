#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Renderer/Buffer.h"

namespace Orion
{
    class OpenGLVertexBuffer final : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(const void* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;
        OE_NON_COPYABLE(OpenGLVertexBuffer);

        void Bind() const override;
        void Unbind() const override;

        void SetLayout(const BufferLayout& layout) override;
        [[nodiscard]] const BufferLayout& GetLayout() const override;

    private:
        uint32_t m_RendererID{0};
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer final : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;
        OE_NON_COPYABLE(OpenGLIndexBuffer);

        [[nodiscard]] uint32_t GetCount() const override
        {
            return m_Count;
        }

        void Bind() const override;
        void Unbind() const override;

    private:
        uint32_t m_RendererID{0};
        uint32_t m_Count;
    };

} // namespace Orion
