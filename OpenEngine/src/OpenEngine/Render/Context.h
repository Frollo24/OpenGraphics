#pragma once

#include "OpenEngine/Core/Build.h"

namespace OpenGraphics
{
    class OPEN_API Context
    {
    public:
        Context(void* windowHandle);
        ~Context();

        void Init();
    private:
        void* m_WindowHandle = nullptr;
    };
}
