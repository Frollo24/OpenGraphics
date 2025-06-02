#include "ogpch.h"
#include "SceneRenderer.h"

#include "OpenEngine/Scene/CameraComponent.h"

namespace OpenGraphics
{
    SceneRenderer::SceneRenderer(const Scene* scene)
        : m_Scene(const_cast<Scene*>(scene))
    {

    }

    std::vector<const RenderCamera*> SceneRenderer::GetCameras() const
    {
        std::vector<const RenderCamera*> cameras;

        const bool hasEditorCamera = m_EditorCamera != nullptr;
        if (hasEditorCamera)
            cameras.reserve(m_Scene->GetGameObjects().size() + 1);
        else
            cameras.reserve(m_Scene->GetGameObjects().size());

        if (hasEditorCamera)
            cameras.push_back(m_EditorCamera);

        for (const auto& gameObject : m_Scene->GetGameObjects())
        {
            if (const auto& camera = gameObject->GetComponent<CameraComponent>())
            {
                cameras.push_back(camera->GetRenderCamera());
            }
        }

        return cameras;
    }
}
