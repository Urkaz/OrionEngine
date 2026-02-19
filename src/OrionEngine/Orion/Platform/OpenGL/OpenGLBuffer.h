#pragma once

#include "Orion/Renderer/Buffer.h"

namespace Orion
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetLayout(const BufferLayout& layout) override;
        virtual const BufferLayout& GetLayout() override;

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();

        virtual uint32_t GetCount() const override
        {
            return m_Count;
        }

        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };

} // namespace Orion
