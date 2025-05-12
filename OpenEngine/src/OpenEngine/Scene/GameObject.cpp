#include "ogpch.h"
#include "GameObject.h"

namespace OpenGraphics
{
    GameObject::GameObject(const Vector3D& position, const Transform* parent)
        : m_Transform(new Transform(position))
    {
        m_Transform->Parent(const_cast<Transform*>(parent));
        OnCreate();
    }

    GameObject::GameObject(const float x, const float y, const float z)
        : m_Transform(new Transform(Vector3D(x, y, z)))
    {
        OnCreate();
    }

    GameObject::~GameObject()
    {
        OnDestroy();
        delete m_Transform;
    }

    void GameObject::OnCreate()
    {
        m_Transform->OnCreate();

        for (const auto& component : m_ComponentList)
            component->OnCreate();
    }

    void GameObject::OnUpdate()
    {
        m_Transform->OnUpdate();

        for (const auto& component : m_ComponentList)
            component->OnUpdate();
    }

    void GameObject::OnRender()
    {
        for (const auto& component : m_ComponentList)
            component->OnRender();
    }

    void GameObject::OnDestroy()
    {
        m_Transform->OnDestroy();

        for (const auto& component : m_ComponentList)
            delete component;

        m_ComponentList.clear();
    }
}
