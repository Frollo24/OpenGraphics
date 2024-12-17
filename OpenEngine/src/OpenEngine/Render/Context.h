#pragma once

#include "OpenEngine/Core/Base.h"

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
