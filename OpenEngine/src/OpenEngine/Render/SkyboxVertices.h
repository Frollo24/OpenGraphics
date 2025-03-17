#pragma once

#include <array>
#include "OpenEngine/Math/Vector3D.h"

namespace OpenGraphics
{
    inline std::array<Vector3D, 8> g_SkyboxVertices = {
        Vector3D(-1, -1, -1),
        Vector3D(-1, -1,  1),
        Vector3D(-1,  1, -1),
        Vector3D(-1,  1,  1),
        Vector3D( 1, -1, -1),
        Vector3D( 1, -1,  1),
        Vector3D( 1,  1, -1),
        Vector3D( 1,  1,  1),
    };

    inline std::array<uint32_t, 36> g_SkyboxIndices = {
        0, 1, 2, 2, 1, 3,  // X-Neg face
        4, 6, 5, 5, 6, 7,  // X-Pos face
        0, 4, 1, 1, 4, 5,  // Y-Neg face
        2, 3, 6, 6, 3, 7,  // Y-Pos face
        0, 2, 4, 4, 2, 6,  // Z-Neg face
        1, 5, 3, 3, 5, 7,  // Z-Pos face
    };
}