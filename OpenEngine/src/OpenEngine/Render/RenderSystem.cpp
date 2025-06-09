#include "ogpch.h"
#include "RenderSystem.h"
#include "RenderCommand.h"

namespace OpenGraphics
{
    void RenderSystem::Init()
    {
        s_RenderWorkflow = new RenderWorkflow();

        RenderWorkflow::InitResources();

        // TODO: init render system common data
    }

    void RenderSystem::BeginFrame()
    {
        RenderCommand::BeginFrame();
    }

    void RenderSystem::RenderScene(const SceneRenderer& sceneRenderer)
    {
        const auto& cameras = sceneRenderer.GetCameras();
        s_SceneRenderer = const_cast<SceneRenderer*>(&sceneRenderer);
        s_RenderWorkflow->SetSceneRenderer(s_SceneRenderer);
        s_RenderWorkflow->Render(cameras);
    }

    void RenderSystem::EndFrame()
    {
        RenderCommand::EndFrame();
    }

    void RenderSystem::Shutdown()
    {
        delete s_RenderWorkflow;
        s_RenderWorkflow = nullptr;

        // TODO: delete render system common data
    }
}
