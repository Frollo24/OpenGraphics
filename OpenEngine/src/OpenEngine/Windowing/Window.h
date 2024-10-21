#pragma once

#include "OpenEngine/Core/Build.h"
#include "OpenEngine/Render/Context.h"

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
        Context* m_Context = nullptr;
        GLFWwindow* m_WindowHandle = nullptr;
    };
}
