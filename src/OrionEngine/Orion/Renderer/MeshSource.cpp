#include "Orion/Renderer/MeshSource.h"

#include "Orion/Core/FileSystem/VirtualPath.h"

#include <algorithm>

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

    std::string ResolveTexturePath(std::string_view modelDirectory, std::string_view texturePath)
    {
        if (texturePath.empty())
            return {};

        std::string relative(texturePath);
        std::ranges::replace(relative, '\\', '/'); // Normalize to forward slashes

        if (std::optional<std::string> path = VirtualPath::Normalize(VirtualPath::Join(modelDirectory, relative)))
            return *path;

        // Normalize failed, look next to the model using the filename
        return VirtualPath::Normalize(VirtualPath::Join(modelDirectory, VirtualPath::Filename(relative))).value_or("");
    }
} // namespace Orion
