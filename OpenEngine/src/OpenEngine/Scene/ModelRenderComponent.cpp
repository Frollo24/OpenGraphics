#include "ogpch.h"
#include "ModelRenderComponent.h"

#include "OpenEngine/Render/RenderSystem.h"

namespace OpenGraphics
{
    ModelRenderComponent::~ModelRenderComponent()
    {
        delete m_Model;
    }

    void ModelRenderComponent::OnRender()
    {
        if (m_Model) [[likely]]
            RenderSystem::ActiveSceneRenderer()->DrawModel(*m_Model, *GetGameObject().GetTransform());
        // for (const auto& mesh : m_Model->GetMeshes())
        //     mesh.Render();
    }
}
