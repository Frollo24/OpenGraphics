#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/Scene.h"

namespace OpenGraphics
{
    class OPEN_API SceneRenderer
    {
    public:
        SceneRenderer(const Scene* scene);

        [[nodiscard]] std::vector<const RenderCamera*> GetCameras() const;

        // TODO: improve interface
        [[nodiscard]] inline const RenderCamera* GetEditorCamera() const { return m_EditorCamera; }
        void SetEditorCamera(RenderCamera* camera) { m_EditorCamera = camera; }

    private:
        Scene* m_Scene = nullptr;
        RenderCamera* m_EditorCamera = nullptr;  // TODO: promote to a Editor Camera
    };
}