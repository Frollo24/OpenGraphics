#pragma once

#include "OpenEngine/Core/Base.h"

namespace OpenGraphics
{
    enum class PrimitiveTopology : uint8_t
    {
        Points,
        Lines,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan
    };
}