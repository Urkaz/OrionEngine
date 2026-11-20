#include "Orion/Renderer/MeshRenderData.h"

#include "Orion/Core/Base.h"
#include "Orion/Core/FileSystem/FileSystem.h"
#include "Orion/Renderer/Buffer.h"
#include "Orion/Renderer/Texture.h"
#include "Orion/Renderer/VertexArray.h"

#include <unordered_map>

namespace Orion
{
    MeshRenderData::MeshRenderData(const Ref<MeshSource>& source) : m_Source(source)
    {
        OE_PROFILE_FUNCTION();
        OE_CORE_ASSERT(!source->Vertices.empty() && !source->Indices.empty(), "MeshSource has no geometry");

        m_VertexArray = VertexArray::Create();

        Ref<VertexBuffer> vertexBuffer =
            VertexBuffer::Create(source->Vertices.data(),
                                 static_cast<uint32_t>(source->Vertices.size() * sizeof(Vertex)));
        vertexBuffer->SetLayout(Vertex::GetLayout());
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        Ref<IndexBuffer> indexBuffer =
            IndexBuffer::Create(source->Indices.data(), static_cast<uint32_t>(source->Indices.size()));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        Ref<Texture2D> whiteTexture                 = Texture2D::Create(1, 1);
        constexpr std::array<uint8_t, 4> whitePixel = {255, 255, 255, 255};
        whiteTexture->SetData(whitePixel.data(), static_cast<uint32_t>(whitePixel.size()));

        Ref<Texture2D> flatNormalTexture                 = Texture2D::Create(1, 1);
        constexpr std::array<uint8_t, 4> flatNormalPixel = {128, 128, 255, 255};
        flatNormalTexture->SetData(flatNormalPixel.data(), static_cast<uint32_t>(flatNormalPixel.size()));

        constexpr size_t CheckerSize  = 8;
        Ref<Texture2D> magentaTexture = Texture2D::Create(CheckerSize, CheckerSize);
        constexpr auto magentaPixels  = [] {
            constexpr size_t CheckerCell = 4;
            std::array<uint8_t, CheckerSize * CheckerSize * 4> pixels{};
            constexpr std::array<uint8_t, 4> Magenta = {255, 0, 255, 255};
            constexpr std::array<uint8_t, 4> Black   = {0, 0, 0, 255};

            for (size_t x = 0; x < CheckerSize; ++x)
            {
                for (size_t y = 0; y < CheckerSize; ++y)
                {
                    const auto& color  = ((x / CheckerCell) + (y / CheckerCell)) % 2 ? Magenta : Black;
                    const size_t index = (x + y * CheckerSize) * 4;
                    pixels[index + 0]  = color[0];
                    pixels[index + 1]  = color[1];
                    pixels[index + 2]  = color[2];
                    pixels[index + 3]  = color[3];
                }
            }
            return pixels;
        }(); // the () at the end calls the lambda immediately, keeps it constexpr
        magentaTexture->SetData(magentaPixels.data(), static_cast<uint32_t>(magentaPixels.size()));

        std::unordered_map<std::string, Ref<Texture2D>> textureCache;
        auto loadTexture = [&source, &textureCache](const std::string& path,
                                                    bool srgb,
                                                    const Ref<Texture2D>& fallback,
                                                    const Ref<Texture2D>& missing) -> Ref<Texture2D> {
            if (path.empty())
                return fallback;

            const std::string cacheKey = path + (srgb ? "|srgb" : "|linear");
            if (const auto it = textureCache.find(cacheKey); it != textureCache.end())
                return it->second;

            Ref<Texture2D> texture = missing;
            if (const auto it = source->EmbeddedTextures.find(path); it != source->EmbeddedTextures.end())
            {
                if (Ref<Texture2D> loaded = Texture2D::CreateFromMemory(it->second.data(), it->second.size(), srgb))
                    texture = loaded;
            }
            else
            {
                std::string filePath = ResolveTexturePath(source->Directory, path);
                if (FileSystem::Exists(filePath))
                {
                    if (Ref<Texture2D> loaded = Texture2D::Create(filePath, srgb))
                        texture = loaded;
                }
                else
                {
                    OE_CORE_LOG(Warn, "Texture file not found: {0}", filePath);
                }
            }
            textureCache[cacheKey] = texture;
            return texture;
        };

        m_Materials.reserve(source->Materials.size());
        for (const auto& material : source->Materials)
        {
            MeshRenderMaterial& renderMaterial = m_Materials.emplace_back();
            renderMaterial.Albedo              = loadTexture(material.AlbedoPath, true, whiteTexture, magentaTexture);
            renderMaterial.Normal       = loadTexture(material.NormalPath, false, flatNormalTexture, flatNormalTexture);
            renderMaterial.HasNormalMap = renderMaterial.Normal != flatNormalTexture;
        }
    }
} // namespace Orion
