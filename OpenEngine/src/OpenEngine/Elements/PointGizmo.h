#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Math/Vector3D.h"
#include "OpenEngine/Math/Color.h"

#include "OpenEngine/Render/Buffer.h"
#include "OpenEngine/Render/VertexArray.h"


namespace OpenGraphics
{
    class OPEN_API PointGizmo
    {
    public:
        PointGizmo(const Vector3D& position, const Color& color);
        ~PointGizmo();

        void Render() const;

    private:
        VertexArray* m_VertexArray = nullptr;
        VertexAttribBinding m_VertexAttribBinding = {};
        Buffer* m_VertexBuffer = nullptr;
    };

}
