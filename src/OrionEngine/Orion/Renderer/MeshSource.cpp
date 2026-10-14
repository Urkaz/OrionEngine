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
} // namespace Orion
