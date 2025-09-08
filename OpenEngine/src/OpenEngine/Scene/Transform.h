#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/Component.h"
#include "OpenEngine/Math/Matrix4x4.h"

namespace OpenGraphics
{
    class OPEN_API Transform final : public Component
    {
    public:
        Transform() = default;
        Transform(const Vector3D& position);
        Transform(const Vector3D& position, Transform* parent);

        [[nodiscard]] inline const Matrix4x4& GetLocalMatrix() const { return m_LocalMatrix; }
        [[nodiscard]] inline const Matrix4x4& GetModelMatrix() const { return m_ModelMatrix; }

        void OnUpdate() override;

        void Translate(const Vector3D& translation);
        void Rotate(const Vector3D& axis, const float& angle);
        void Scale(const Vector3D& scale);
        void Parent(Transform* parent);
        void Unparent();

        bool IsDeepChildOf(Transform* deepParent) const;

    private:
        void UpdateModelMatrix();
        void SetDirty();

    private:
        Matrix4x4 m_LocalMatrix = Matrix4x4::identity;
        Matrix4x4 m_ModelMatrix = Matrix4x4::identity;
        Transform* m_Parent = nullptr;
        std::vector<Transform*> m_Children;
        bool m_Dirty = false;
    };
}
