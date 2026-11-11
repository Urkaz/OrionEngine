#include "oepch.h"

#include "Orion/Renderer/MeshSource.h"

namespace Orion
{
    void MeshSource::UpdateNodeTransforms()
    {
        for (uint32_t i = 0; i < Nodes.size(); ++i)
        {
            MeshNode& node = Nodes[i];
            if (node.Parent == MeshNode::NoParent)
            {
                node.WorldTransform = node.LocalTransform;
            }
            else
            {
                OE_CORE_ASSERT(node.Parent < i, "A node's parent must come before the node!");
                node.WorldTransform = Nodes[node.Parent].WorldTransform * node.LocalTransform;
            }
        }
    }

    std::filesystem::path ResolveTexturePath(const std::filesystem::path& modelDirectory, std::string texturePath)
    {
        if (texturePath.empty())
            return {};
        std::ranges::replace(texturePath, '\\', '/'); // Normalize to forward slashes
        return modelDirectory / texturePath;
    }
} // namespace Orion
