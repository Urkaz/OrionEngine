#include "oepch.h"
#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"

namespace Orion
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            OE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLVertexArray();
        default:
            break;
        }

        OE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // namespace Orion
