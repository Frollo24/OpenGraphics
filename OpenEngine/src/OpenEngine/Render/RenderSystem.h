#pragma once

#include "OpenEngine/Core/Base.h"
#include "RenderWorkflow.h"

namespace OpenGraphics
{
    class OPEN_API RenderSystem
    {
    public:
        static void Init();
        static void BeginFrame();
        static void RenderScene();
        static void EndFrame();
        static void Shutdown();

        // TODO: temporary
        static void RenderScene(const std::vector<RenderCamera*>& cameras);

    private:
        // NOTE: this should later be a RenderWorflowAsset that can be
        // either selected from the editor or retrieved from the runtime
        inline static RenderWorkflow* s_RenderWorkflow = nullptr;
    };
}
