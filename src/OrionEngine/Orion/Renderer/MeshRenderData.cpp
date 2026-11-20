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

        std::unordered_map<std::string, Ref<Texture2D>> textureCache;
        auto loadTexture = [&source, &textureCache](const std::string& path,
                                                    bool srgb,
                                                    const Ref<Texture2D>& fallback) -> Ref<Texture2D> {
            if (path.empty())
                return fallback;

            const std::string cacheKey = path + (srgb ? "|srgb" : "|linear");
            if (const auto it = textureCache.find(cacheKey); it != textureCache.end())
                return it->second;

            Ref<Texture2D> texture = fallback;
            if (const auto it = source->EmbeddedTextures.find(path); it != source->EmbeddedTextures.end())
            {
                texture = Texture2D::CreateFromMemory(it->second.data(), it->second.size(), srgb);
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
                    OE_CORE_LOG(Warn, "Texture file not found: {0}", filePath);
            }
            textureCache[cacheKey] = texture;
            return texture;
        };

        m_Materials.reserve(source->Materials.size());
        for (const auto& material : source->Materials)
        {
            MeshRenderMaterial& renderMaterial = m_Materials.emplace_back();
            renderMaterial.Albedo              = loadTexture(material.AlbedoPath, true, whiteTexture);
            renderMaterial.Normal              = loadTexture(material.NormalPath, false, flatNormalTexture);
            renderMaterial.HasNormalMap        = renderMaterial.Normal != flatNormalTexture;
        }

        const auto withNormalMap = std::ranges::count_if(m_Materials, [](const MeshRenderMaterial& material) {
            return material.HasNormalMap;
        });
        OE_CORE_LOG(Info, "Materials: {0}, with normal map: {1}", m_Materials.size(), withNormalMap);
    }
} // namespace Orion
