#pragma once

#include "Orion/Renderer/MeshSource.h"
#include "Orion/Renderer/Texture.h"
#include "Orion/Renderer/VertexArray.h"

namespace Orion
{
    struct MeshRenderMaterial
    {
        Ref<Texture2D> Albedo; // never null: the white fallback if the material has none
        Ref<Texture2D> Normal; // never null: the flat normal fallback
        bool HasNormalMap = false;
    };

    class MeshRenderData
    {
    public:
        explicit MeshRenderData(const Ref<MeshSource>& source);

        [[nodiscard]] const Ref<MeshSource>& GetSource() const
        {
            return m_Source;
        }
        [[nodiscard]] const Ref<VertexArray>& GetVertexArray() const
        {
            return m_VertexArray;
        }
        [[nodiscard]] const MeshRenderMaterial& GetMaterial(uint32_t index) const
        {
            OE_CORE_ASSERT(index < m_Materials.size(), "Material index out of range!");
            return m_Materials[index];
        }

    private:
        Ref<MeshSource> m_Source;
        Ref<VertexArray> m_VertexArray;
        std::vector<MeshRenderMaterial> m_Materials;
    };
} //namespace Orion
