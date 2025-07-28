#include "ogpch.h"
#include "Component.h"

namespace OpenGraphics
{
    Component::Component(GameObject* gameObject)
        : m_GameObject(gameObject)
    {
        OnCreate();
    }

    Component::~Component()
    {
        OnDestroy();
    }


}
