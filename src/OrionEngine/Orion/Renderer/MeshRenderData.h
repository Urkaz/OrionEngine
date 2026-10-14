#pragma once

#include "Orion/Renderer/MeshSource.h"
#include "Orion/Renderer/VertexArray.h"

namespace Orion
{
    class MeshRenderData
    {
    public:
        explicit MeshRenderData(const Ref<MeshSource>& source);

        const Ref<MeshSource>& GetSource() const
        {
            return m_Source;
        }
        const Ref<VertexArray>& GetVertexArray() const
        {
            return m_VertexArray;
        }

    private:
        Ref<MeshSource> m_Source;
        Ref<VertexArray> m_VertexArray;
    };
} //namespace Orion
