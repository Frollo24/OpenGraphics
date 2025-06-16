#include "ogpch.h"
#include "PointGizmo.h"
#include "OpenEngine/Render/RenderStructs.h"
#include "OpenEngine/Render/RenderCommand.h"

namespace OpenGraphics
{
    PointGizmo::PointGizmo(const Vector3D& position, const Color& color)
    {
        struct ColorVertex
        {
            Vector3D position;
            Color color;
        } data {position, color};

        BufferDescription vertexDescription = {};
        vertexDescription.Type = BufferType::Vertex;
        vertexDescription.Size = sizeof(ColorVertex) * 2;
        vertexDescription.Data = (void*)(&data);
        m_VertexBuffer = new Buffer(vertexDescription);

        m_VertexAttribBinding = GetGizmoVertexBinding();
        m_VertexArray = new VertexArray({m_VertexAttribBinding});
    }

    void PointGizmo::Render() const
    {
        RenderCommand::BindVertexArray(m_VertexArray);
        RenderCommand::SetVertexBuffer(m_VertexBuffer, m_VertexAttribBinding);
        RenderCommand::SetPrimitiveTopology(PrimitiveTopology::Points);
        RenderCommand::Draw(0, 1);
    }
}