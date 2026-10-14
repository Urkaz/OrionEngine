#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Renderer/Buffer.h"
#include "Orion/Renderer/VertexArray.h"

#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace Orion
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        glm::vec2 TexCoord;

        // Layout of the vertex buffer. Must describe EXACTLY the memory layout of this struct
        // (the attribute locations are 0 to 4 in the same order)
        static BufferLayout GetLayout()
        {
            // clang-format off
            return {{ShaderDataType::Float3, "a_Position"},
                    {ShaderDataType::Float3, "a_Normal"},
                    {ShaderDataType::Float3, "a_Tangent"},
                    {ShaderDataType::Float3, "a_Bitangent"},
                    {ShaderDataType::Float2, "a_TexCoord"}};
            // clang-format on
        };
    };
    static_assert(sizeof(Vertex) == 14 * sizeof(float), "Vertex must have no padding");

    // A range of the shared v ertex/index buffers. Its indices starts at 0 (relative to VertexOffset)
    struct Submesh
    {
        uint32_t VertexOffset  = 0;
        uint32_t FirstIndex    = 0;
        uint32_t VertexCount   = 0;
        uint32_t IndexCount    = 0; // 0 for meshes that are not triangles (kept as placeholders)
        uint32_t MaterialIndex = 0;
        std::string Name;
    };

    // A node of the scene tree. Meshes are references by index. The world transform lives here, not in the submesh,
    // because several nodes can reference the same submesh.
    struct MeshNode
    {
        static constexpr uint32_t NoParent = 0xFFFFFFFF;

        std::string Name;
        uint32_t Parent = NoParent;
        std::vector<uint32_t> Children;
        std::vector<uint32_t> Submeshes; // indices into MeshSource::Submeshes

        glm::mat4 LocalTransform{1.0f};
        glm::mat4 WorldTransform{1.0f}; // relative to the model root; see MeshSource::UpdateNodeTransform
    };

    // Texture paths are kept as written in the file (relative to the model).
    struct MeshMaterial
    {
        std::string Name;
        std::string AlbedoPath;
        std::string NormalPath;
    };

    // The imported model as plain CPU data
    struct MeshSource
    {
        std::string Name;
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::vector<Submesh> Submeshes;
        std::vector<MeshNode> Nodes; // Nodes[0] is the root; a parent always comes before its children
        std::vector<MeshMaterial> Materials;

        // Recomputes ever node's WorldTransform from the local transforms -> world = parent.world * local
        void UpdateNodeTransforms();
    };
} // namespace Orion
