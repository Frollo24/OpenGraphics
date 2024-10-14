#pragma once

#include "OpenEngine/Core/Build.h"

struct GLFWwindow;

namespace OpenGraphics
{
    class OPEN_API Window
    {
    public:
        Window();
        ~Window();

        bool ShouldClose() const;
        void SwapBuffers() const;

    private:
        GLFWwindow* m_WindowHandle = nullptr;
    };
}
