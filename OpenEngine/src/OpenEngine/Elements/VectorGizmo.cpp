#include "ogpch.h"
#include "VectorGizmo.h"
#include "OpenEngine/Render/RenderStructs.h"
#include "OpenEngine/Render/RenderCommand.h"

namespace OpenGraphics
{
    VectorGizmo::VectorGizmo(const Vector3D& start, const Vector3D& end, const Color& color)
    {
        ColorVertex first = {start, color};
        ColorVertex second = {end, color};

        InitVertexData(first, second);
    }

    VectorGizmo::VectorGizmo(const Vector3D& positionStart, const Vector3D& positionEnd, const Color& colorStart, const Color& colorEnd)
    {
        ColorVertex first = {positionStart, colorStart};
        ColorVertex second = {positionEnd, colorEnd};

        InitVertexData(first, second);
    }

    void VectorGizmo::InitVertexData(const ColorVertex& first, const ColorVertex& second)
    {
        BufferDescription vertexDescription = {};
        vertexDescription.Type = BufferType::Vertex;
        vertexDescription.Size = sizeof(ColorVertex) * 2;
        vertexDescription.Data = (void*)(std::array<ColorVertex, 2>{first, second}.data());
        m_VertexBuffer = new Buffer(vertexDescription);

        m_VertexAttribBinding = GetGizmoVertexBinding();
        m_VertexArray = new VertexArray({m_VertexAttribBinding});
    }

    void VectorGizmo::Render() const
    {
        RenderCommand::BindVertexArray(m_VertexArray);
        RenderCommand::SetVertexBuffer(m_VertexBuffer, m_VertexAttribBinding);
        RenderCommand::SetPrimitiveTopology(PrimitiveTopology::Lines);
        RenderCommand::Draw(0, 2);
        RenderCommand::SetPrimitiveTopology(PrimitiveTopology::Points);
        RenderCommand::Draw(1, 1);
        // RenderCommand::DrawLines(2);
        // RenderCommand::DrawPoints(1, 1);
    }
}
