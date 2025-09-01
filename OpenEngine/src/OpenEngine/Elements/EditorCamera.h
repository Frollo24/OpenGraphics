#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Render/RenderCamera.h"

namespace OpenGraphics
{
    class OPEN_API EditorCamera final : public RenderCamera
    {
    public:
        EditorCamera() = default;
        EditorCamera(const Matrix4x4& projection) : RenderCamera(projection) {}
        EditorCamera(const float fov, const float aspect, const float nearPlane, const float farPlane)
            : RenderCamera(Matrix4x4::Perspective(fov, aspect, nearPlane, farPlane)) {}
        virtual ~EditorCamera() = default;

        void SetView(const Matrix4x4& view) { m_View = view; }
        void SetPosition(const Vector3D& position) { m_Position = position; }
    };
}
