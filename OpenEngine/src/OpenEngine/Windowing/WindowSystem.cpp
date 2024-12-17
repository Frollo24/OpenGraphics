#include "ogpch.h"
#include "WindowSystem.h"

#include <GLFW/glfw3.h>

namespace OpenGraphics
{
    void WindowSystem::Init()
    {
        glfwInit();
    }

    void WindowSystem::Shutdown()
    {
        glfwTerminate();
    }

    Window* WindowSystem::Create()
    {
        return new Window();
    }

    void WindowSystem::Destroy(const Window* window)
    {
        delete window;
    }
}
