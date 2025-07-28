#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Render/Context.h"

struct GLFWwindow;

namespace OpenGraphics
{
    class OPEN_API Window
    {
    public:
        Window();
        ~Window();

        [[nodiscard]] bool ShouldClose() const;
        void SwapBuffers() const;

    private:
        Context* m_Context = nullptr;
        GLFWwindow* m_WindowHandle = nullptr;
    };
}
