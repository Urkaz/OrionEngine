#include "oepch.h"
#include "Orion/Renderer/Buffer.h"

#include "Orion/Platform/OpenGL/OpenGLBuffer.h"
#include "Orion/Renderer/Renderer.h"

namespace Orion
{
    Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            OE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLVertexBuffer>(vertices, size);
        default:
            break;
        }

        OE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            OE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLIndexBuffer>(indices, size);
        default:
            break;
        }

        OE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // namespace Orion
