#include "oepch.h"
#include "Orion/Renderer/Texture.h"

#include "Orion/Platform/OpenGL/OpenGLTexture.h"
#include "Orion/Renderer/Renderer.h"

namespace Orion
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            OE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(width, height);
        default:
            break;
        }

        OE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            OE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path);
        default:
            break;
        }

        OE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // namespace Orion
