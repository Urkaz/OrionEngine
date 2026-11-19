#include "oepch.h"
#include "Orion/Renderer/Texture.h"

#include "Orion/Core/FileSystem/FileSystem.h"
#include "Orion/Platform/OpenGL/OpenGLTexture.h"
#include "Orion/Renderer/Renderer.h"
#include <cstdint>

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
            return CreateRef<OpenGLTexture2D>(width, height);
        default:
            break;
        }

        OE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path, bool srgb)
    {
        const std::optional<std::vector<uint8_t>> data = FileSystem::ReadBinary(path);
        if (!data)
        {
            OE_CORE_LOG(Error, "Could not read texture \"{0}\"", path);
            return nullptr;
        }
        return CreateFromMemory(data->data(), data->size(), srgb);
    }

    Ref<Texture2D> Texture2D::CreateFromMemory(const void* data, size_t size, bool srgb)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            OE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLTexture2D>(data, size, srgb);
        default:
            break;
        }

        OE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // namespace Orion
