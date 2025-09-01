#include "ogpch.h"
#include "CameraComponent.h"

namespace OpenGraphics
{
    void CameraComponent::OnUpdate()
    {
        m_SceneCamera->Update();
    }
}
