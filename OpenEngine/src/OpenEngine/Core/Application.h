#pragma once

#include "OpenEngine/Core/Build.h"

namespace OpenGraphics
{
    class OPEN_API Application
    {
    public:
        Application() = default;
        virtual ~Application() = default;

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;

        void Run();
    };

    Application* CreateApplication();
}
