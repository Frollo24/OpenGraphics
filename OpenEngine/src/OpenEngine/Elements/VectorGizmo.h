#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Math/Vector3D.h"
#include "OpenEngine/Math/Color.h"

#include "OpenEngine/Render/Buffer.h"
#include "OpenEngine/Render/VertexArray.h"


namespace OpenGraphics
{
    class OPEN_API VectorGizmo
    {
    public:
        VectorGizmo(const Vector3D& start, const Vector3D& end, const Color& color);
        VectorGizmo(const Vector3D& positionStart, const Vector3D& positionEnd, const Color& colorStart, const Color& colorEnd);

        void Render() const;

    private:
        struct ColorVertex
        {
            Vector3D position;
            Color color;
        };

        void InitVertexData(const ColorVertex& first, const ColorVertex& second);

    private:
        VertexArray* m_VertexArray = nullptr;
        VertexAttribBinding m_VertexAttribBinding = {};
        Buffer* m_VertexBuffer = nullptr;
    };

}
