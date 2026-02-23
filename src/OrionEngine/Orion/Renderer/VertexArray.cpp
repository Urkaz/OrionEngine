#include "oepch.h"
#include "Orion/Renderer/VertexArray.h"

#include "Orion/Platform/OpenGL/OpenGLVertexArray.h"
#include "Orion/Renderer/Renderer.h"

namespace Orion
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            OE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLVertexArray>();
        default:
            break;
        }

        OE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // namespace Orion
