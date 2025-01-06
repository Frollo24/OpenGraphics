#pragma once

#include "OpenEngine/Math/Vector2D.h"
#include "OpenEngine/Math/Vector3D.h"

namespace OpenGraphics
{
    struct Vertex  // TODO: expand
    {
        Vector3D Position;
        Vector3D Color;
    };

    using Index = uint32_t;

    inline VertexAttribBinding GetRenderEntityVertexBinding()
    {
        return VertexAttribBinding{
            {ShaderDataType::Float3, "aPosition"},
            {ShaderDataType::Float3, "aColor"},
        };
    }
}