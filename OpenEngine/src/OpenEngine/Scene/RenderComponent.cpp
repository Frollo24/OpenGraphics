#include "ogpch.h"
#include "RenderComponent.h"

#include "OpenEngine/Render/RenderSystem.h"

namespace OpenGraphics
{
    RenderComponent::~RenderComponent()
    {
        delete m_Model;
    }

    void RenderComponent::OnRender()
    {
        if (m_Model) [[likely]]
            RenderSystem::ActiveSceneRenderer()->DrawModel(*m_Model, *GetGameObject().GetTransform());
            // for (const auto& mesh : m_Model->GetMeshes())
            //     mesh.Render();
    }
}
