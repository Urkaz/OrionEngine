#include "oepch.h"
#include "Buffer.h"
#include "MeshRenderData.h"
#include "Orion/Renderer/VertexArray.h"

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
    }
} // namespace Orion
