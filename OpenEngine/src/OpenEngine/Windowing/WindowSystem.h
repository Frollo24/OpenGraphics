#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Windowing/Window.h"

namespace OpenGraphics
{
    class OPEN_API WindowSystem
    {
    public:
        static void Init();
        static void Shutdown();

        static Window* Create();
        static void Destroy(const Window* window);
    };
}
