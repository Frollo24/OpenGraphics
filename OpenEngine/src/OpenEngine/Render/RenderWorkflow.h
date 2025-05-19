#pragma once

#include "OpenEngine/Core/Base.h"
#include "RenderCamera.h"

namespace OpenGraphics
{
    class OPEN_API RenderWorkflow
    {
    public:
        RenderWorkflow() = default;
        virtual ~RenderWorkflow() = default;

        virtual void Render(const std::vector<const RenderCamera*>& cameras);

        // HACK: this should be managed by asset loading and render system common data
        static void InitResources();
        static void DeleteResources();

    protected:
        void DrawGameObjects(const RenderCamera* camera);
        void DrawSkybox(const RenderCamera* camera);
        void DrawGizmos(const RenderCamera* camera);
    };
} // OpenGraphics
