#include "ogpch.h"
#include "RenderEntity.h"
#include "RenderCommand.h"

namespace OpenGraphics
{
    RenderEntity::RenderEntity(const std::vector<Vertex> &vertices, const PrimitiveTopology& topology)
        : m_VertexCount(vertices.size()), m_Topology(topology)
    {
        const void* vertexData = vertices.data();
        InitHandles(vertexData, nullptr);
    }

    RenderEntity::RenderEntity(const std::vector<Vertex> &vertices, const std::vector<Index> &indices,
        const PrimitiveTopology& topology)
        : m_VertexCount(vertices.size()), m_IndexCount(indices.size()), m_Topology(topology)
    {
        const void* vertexData = vertices.data();
        const void* indexData = indices.data();
        InitHandles(vertexData, indexData);
    }

    RenderEntity::~RenderEntity()
    {
        DeleteHandles();
    }

    void RenderEntity::Render() const
    {
        RenderCommand::BindVertexArray(m_VertexArray);
        RenderCommand::SetPrimitiveTopology(m_Topology);
        RenderCommand::SetVertexBuffer(m_VertexBuffer, m_VertexAttribBinding);

        if (m_IndexBuffer)
        {
            RenderCommand::SetIndexBuffer(m_IndexBuffer);
            RenderCommand::DrawIndexed(m_IndexCount);
        }
        else
        {
            RenderCommand::Draw(0, m_VertexCount);
        }
    }

    void RenderEntity::InitHandles(const void* vertexData, const void* indexData)
    {
        BufferDescription vertexDescription = {};
        vertexDescription.Type = BufferType::Vertex;
        vertexDescription.Size = sizeof(Vertex) * m_VertexCount;
        vertexDescription.Data = const_cast<void*>(vertexData);
        m_VertexBuffer = new Buffer(vertexDescription);

        if (m_IndexCount && indexData)
        {
            BufferDescription indexDescription = {};
            indexDescription.Type = BufferType::Index;
            indexDescription.Size = sizeof(Index) * m_IndexCount;
            indexDescription.Data = const_cast<void*>(indexData);
            m_IndexBuffer = new Buffer(indexDescription);
        }

        m_VertexAttribBinding = GetRenderEntityVertexBinding();
        m_VertexArray = new VertexArray({m_VertexAttribBinding});
    }

    void RenderEntity::DeleteHandles()
    {
        delete m_VertexArray;
        m_VertexArray = nullptr;

        if (!m_IndexBuffer)
        {
            delete m_IndexBuffer;
            m_IndexBuffer = nullptr;
        }

        delete m_VertexBuffer;
        m_VertexBuffer = nullptr;
    }
}
