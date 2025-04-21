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

    void RenderSystem::RenderScene()
    {
        // TODO: get camera vector from a scene renderer

        s_RenderWorkflow->Render({});
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

    void RenderSystem::RenderScene(const std::vector<RenderCamera*>& cameras)
    {
        s_RenderWorkflow->Render(cameras);
    }
}
