#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/Transform.h"

namespace OpenGraphics
{
    class OPEN_API GameObject final
    {
    public:
        GameObject(const Vector3D& position = Vector3D::zero, const Transform* parent = nullptr);
        GameObject(float x, float y, float z);
        ~GameObject();

        [[nodiscard]] inline Transform* GetTransform() const { return m_Transform; }
        inline void SetTransform(Transform* transform) { m_Transform = transform; }

        [[nodiscard]] inline const std::vector<Component*>& GetComponentList() const { return m_ComponentList; }

        void OnCreate();
        void OnUpdate();
        void OnRender();
        void OnDestroy();

        template <class T> T* AddComponent() requires std::is_base_of_v<Component, T>
        {
            if (typeid(T) == typeid(Transform))
                return nullptr;

            auto newComponent = new T();
            newComponent->SetGameObject(*this);
            m_ComponentList.push_back(newComponent);

            return newComponent;
        }

        template <class T> T* GetComponent() requires std::is_base_of_v<Component, T>
        {
            if (typeid(T) == typeid(Transform))
                return (T*)m_Transform;

            for (auto& component : m_ComponentList)
                if (dynamic_cast<T*>(component))
                    return (T*)component;

            return nullptr;
        }

    private:
        Transform* m_Transform;
        std::vector<Component*> m_ComponentList;
    };
}
