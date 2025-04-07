#pragma once

#include "OpenEngine/Core/Base.h"

namespace OpenGraphics
{
    class OPEN_API RenderWorkflow
    {
    public:
        RenderWorkflow() = default;
        virtual ~RenderWorkflow() = default;

        virtual void Render();

        // HACK: this should be managed by asset loading and render system common data
        static void InitResources();
        static void DeleteResources();

    protected:
        void DrawGameObjects();
        void DrawSkybox();
        void DrawGizmos();
    };
} // OpenGraphics
