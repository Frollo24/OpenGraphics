#include "Window.h"

#include <GLFW/glfw3.h>

namespace OpenGraphics
{
    Window::Window()
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        m_WindowHandle = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);

        m_Context = new Context(m_WindowHandle);
        m_Context->Init();
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_WindowHandle);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_WindowHandle);
    }

    void Window::SwapBuffers() const
    {
        glfwPollEvents();
        glfwSwapBuffers(m_WindowHandle);
    }
} // OpenGraphics