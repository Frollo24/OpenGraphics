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

    void RenderSystem::RenderScene(const Scene* scene)
    {
        // TODO: get camera vector from a scene renderer
        const RenderCamera* camera = scene->GetCamera();

        s_RenderWorkflow->Render({camera});
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
