#include "ogpch.h"
#include "Component.h"

namespace OpenGraphics
{
    Component::Component(const GameObject* gameObject)
        : m_GameObject(const_cast<GameObject*>(gameObject))
    {
        OnCreate();
    }

    Component::~Component()
    {
        OnDestroy();
    }


}
