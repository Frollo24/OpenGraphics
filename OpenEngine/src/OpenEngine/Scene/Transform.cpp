#include "ogpch.h"
#include "Transform.h"

namespace OpenGraphics
{
    Transform::Transform(const Vector3D& position)
        : m_LocalMatrix(Matrix4x4(1.0f).Translate(position)), m_ModelMatrix(m_LocalMatrix)
    {
    }

    Transform::Transform(const Vector3D& position, Transform* parent)
        : m_LocalMatrix(Matrix4x4(1.0f).Translate(position)), m_ModelMatrix(m_LocalMatrix)
    {
        Parent(parent);
    }

    void Transform::OnUpdate()
    {
        if (m_Dirty)
            UpdateModelMatrix();
    }

    void Transform::Translate(const Vector3D& translation)
    {
        m_LocalMatrix.Translate(translation);
        SetDirty();
    }

    void Transform::Rotate(const Vector3D& axis, const float& angle)
    {
        m_LocalMatrix.Rotate(axis, angle);
        SetDirty();
    }

    void Transform::Scale(const Vector3D& scale)
    {
        m_LocalMatrix.Scale(scale);
        SetDirty();
    }

    void Transform::Parent(Transform* parent)
    {
        if (!parent)
        {
            Unparent();
            return;
        }

        if (IsDeepChildOf(parent) || parent == this) return;
        if (m_Parent) Unparent();

        m_Parent = parent;
        parent->m_Children.push_back(this);

        m_LocalMatrix = parent->m_ModelMatrix.Inverse() * m_ModelMatrix;
        SetDirty();
    }

    void Transform::Unparent()
    {
        if (!m_Parent) return;

        auto it = std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this);
        if (it != m_Parent->m_Children.end())
            m_Parent->m_Children.erase(it);

        m_Parent = nullptr;
        m_LocalMatrix = m_ModelMatrix;
        SetDirty();
    }

    bool Transform::IsDeepChildOf(Transform* deepParent)
    {
        if (m_Parent == nullptr) return false;
        if (m_Parent == deepParent) return true;
        return m_Parent->IsDeepChildOf(deepParent);
    }

    void Transform::UpdateModelMatrix()
    {
        if (!m_Parent)
        {
            m_ModelMatrix = m_LocalMatrix;
            m_Dirty = false;
            return;
        }

        m_ModelMatrix = m_Parent->m_ModelMatrix * m_LocalMatrix;
        m_Dirty = false;
    }

    void Transform::SetDirty()
    {
        m_Dirty = true;
        if (m_Children.empty())
            return;

        for (const auto& child : m_Children)
            child->SetDirty();
    }
}
