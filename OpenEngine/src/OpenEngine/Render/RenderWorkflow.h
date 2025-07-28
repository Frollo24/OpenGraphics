#pragma once

#include "OpenEngine/Core/Base.h"
#include "RenderCamera.h"
#include "SceneRenderer.h"

namespace OpenGraphics
{
    class OPEN_API RenderWorkflow
    {
    public:
        RenderWorkflow() = default;
        virtual ~RenderWorkflow() = default;

        virtual void Render(const std::vector<const RenderCamera*>& cameras) const;

        [[nodiscard]] inline SceneRenderer* GetSceneRenderer() const { return m_SceneRenderer; }
        inline void SetSceneRenderer(SceneRenderer* sceneRenderer) { m_SceneRenderer = sceneRenderer; }

        // HACK: this should be managed by asset loading and render system common data
        static void InitResources();
        static void DeleteResources();

    protected:
        void DrawGameObjects(const RenderCamera& camera) const;
        void DrawSkybox(const RenderCamera& camera) const;
        void DrawGizmos(const RenderCamera& camera) const;

    private:
        SceneRenderer* m_SceneRenderer = nullptr;
    };
} // OpenGraphics
