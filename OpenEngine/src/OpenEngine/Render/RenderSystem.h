#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Render/SceneRenderer.h"
#include "RenderWorkflow.h"

namespace OpenGraphics
{
    class OPEN_API RenderSystem
    {
    public:
        static void Init();
        static void BeginFrame();
        static void RenderScene(const SceneRenderer& sceneRenderer);
        static void EndFrame();
        static void Shutdown();

        inline static RenderWorkflow* ActiveRenderWorkflow() { return s_RenderWorkflow; }
        inline static const SceneRenderer* ActiveSceneRenderer() { return s_SceneRenderer; }

    private:
        // NOTE: this should later be a RenderWorflowAsset that can be
        // either selected from the editor or retrieved from the runtime
        inline static RenderWorkflow* s_RenderWorkflow = nullptr;
        inline static SceneRenderer* s_SceneRenderer = nullptr;
    };
}
