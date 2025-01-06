#include "ogpch.h"
#include "RenderEntity.h"
#include "RenderCommand.h"

namespace OpenGraphics
{
    RenderEntity::RenderEntity(const std::vector<Vertex> &vertices, const std::vector<Index> &indices)
        : m_VertexCount(vertices.size()), m_IndexCount(indices.size())
    {
        BufferDescription vertexDescription = {};
        vertexDescription.Type = BufferType::Vertex;
        vertexDescription.Size = sizeof(Vertex) * m_VertexCount;
        vertexDescription.Data = (void*)(vertices.data());
        m_VertexBuffer = new Buffer(vertexDescription);

        if (m_IndexCount)
        {
            BufferDescription indexDescription = {};
            indexDescription.Type = BufferType::Index;
            indexDescription.Size = sizeof(Index) * m_IndexCount;
            indexDescription.Data = (void*)(indices.data());
            m_IndexBuffer = new Buffer(indexDescription);
        }

        m_VertexAttribBinding = GetRenderEntityVertexBinding();
        m_VertexArray = new VertexArray({m_VertexAttribBinding});
    }

    RenderEntity::~RenderEntity()
    {
        if (!m_IndexBuffer)
            delete m_IndexBuffer;

        delete m_VertexBuffer;
    }

    void RenderEntity::Render() const
    {
        RenderCommand::BindVertexArray(m_VertexArray);
        RenderCommand::SetVertexBuffer(m_VertexBuffer, m_VertexAttribBinding);

        if (m_IndexBuffer)
        {
            RenderCommand::SetIndexBuffer(m_IndexBuffer);
            RenderCommand::DrawIndexed(m_IndexCount);
        }
        else
        {
            RenderCommand::Draw(m_VertexCount);
        }
    }

}
