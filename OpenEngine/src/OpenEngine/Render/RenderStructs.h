#pragma once

#include "OpenEngine/Math/Vector2D.h"
#include "OpenEngine/Math/Vector3D.h"
#include "OpenEngine/Math/Color.h"

#include "SkyboxVertices.h"

namespace OpenGraphics
{
    struct Vertex
    {
        Vector3D Position;
        Vector3D Normal = Vector3D(0, 0, 1);
        Vector3D Tangent = Vector3D(1, 0, 0);
        Vector3D Bitangent = Vector3D(0, 1, 0);
        Vector2D TexCoord = Vector2D(0, 0);
    };

    using Index = uint32_t;

    inline VertexAttribBinding GetRenderEntityVertexBinding()
    {
        return VertexAttribBinding{
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float3, "a_Tangent"},
            {ShaderDataType::Float3, "a_Bitangent"},
            {ShaderDataType::Float2, "a_TexCoord"},
        };
    }

    struct Material
    {
        // TODO: expand
        Color MainColor;
    };

    inline VertexAttribBinding GetGizmoVertexBinding()
    {
        return VertexAttribBinding{
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };
    }

    inline VertexAttribBinding GetSkyboxVertexBinding() {
        return VertexAttribBinding{
            {ShaderDataType::Float3, "a_Position"},
        };
    }
}