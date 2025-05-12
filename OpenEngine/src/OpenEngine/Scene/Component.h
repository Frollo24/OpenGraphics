#pragma once

#include "OpenEngine/Core/Base.h"

namespace OpenGraphics
{
    class GameObject;

    class OPEN_API Component
    {
    public:
        Component(const GameObject* gameObject = nullptr);
        virtual ~Component();

        [[nodiscard]] inline GameObject& GetGameObject() const { return *m_GameObject; }
        inline void SetGameObject(GameObject* gameObject) { m_GameObject = gameObject; }

        virtual void OnCreate() {}
        virtual void OnUpdate() {}
        virtual void OnRender() {}
        virtual void OnDestroy() {}

    private:
        GameObject* m_GameObject = nullptr;
    };
}
