#include "ogpch.h"
#include "RenderComponent.h"

namespace OpenGraphics
{
    RenderComponent::~RenderComponent()
    {
        delete m_Model;
    }

    void RenderComponent::OnRender()
    {
        if (m_Model) [[likely]]
            for (const auto& mesh : m_Model->GetMeshes())
                mesh.Render();
    }
}
