#pragma once

#include "OpenEngine/Core/Build.h"

namespace OpenGraphics
{
    class OPEN_API RenderCommand
    {
    public:
        static void BeginFrame();
        static void EndFrame();
    };
}
