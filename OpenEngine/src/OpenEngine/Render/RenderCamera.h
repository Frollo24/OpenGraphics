#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Math/Matrix4x4.h"
#include "OpenEngine/Math/Vector3D.h"

#include "OpenEngine/Scene/Scene.h"

namespace OpenGraphics
{
    class OPEN_API RenderCamera
    {
    public:
        RenderCamera() = default;
        RenderCamera(const Matrix4x4& projection) : m_Projection(projection) {}
        RenderCamera(const float& fov, const float& aspect, const float& nearPlane, const float& farPlane)
            : m_Projection(Matrix4x4::Perspective(fov, aspect, nearPlane, farPlane)) {}
        virtual ~RenderCamera() = default;

        [[nodiscard]] inline const Matrix4x4& GetProjection() const { return m_Projection; }
        inline void SetProjection(const Matrix4x4& proj) { m_Projection = proj; }

        [[nodiscard]] inline const Matrix4x4& GetView() const { return m_View; }
        virtual void SetView(const Matrix4x4& view) { m_View = view; }  // TODO: implemented by subclasses

        [[nodiscard]] inline const Vector3D& GetPosition() const { return m_Position; }
        virtual void SetPosition(const Vector3D& position) { m_Position = position; }  // TODO: implemented by subclasses

        // TODO: think about this
        [[nodiscard]] inline const Scene* GetScene() const { return m_Scene; }
        void SetScene(Scene* scene) { m_Scene = scene; }


    private:
        Matrix4x4 m_Projection = Matrix4x4::identity;
        Matrix4x4 m_View = Matrix4x4::identity;
        Vector3D m_Position = Vector3D::zero;

        // TODO: think about this
        Scene* m_Scene = nullptr;
    };
}
