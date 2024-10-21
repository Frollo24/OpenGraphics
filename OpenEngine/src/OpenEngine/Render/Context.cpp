#include "Context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace OpenGraphics
{
    static void OpenGLDebugCallback(GLenum source, GLenum type, unsigned id, GLenum severity, int length, const char* message, const void* userParam)
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << "ERROR: " << message << std::endl; return;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "WARNING: " << message << std::endl; return;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "INFO: " << message << std::endl; return;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "TRACE: " << message << std::endl; return;
            default:
                break;
        }
    }

    Context::Context(void* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        glfwMakeContextCurrent((GLFWwindow*)m_WindowHandle);
    }

    Context::~Context()
    {
        glfwMakeContextCurrent(nullptr);
    }

    void Context::Init()
    {
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (!status)
        {
            std::cerr << "OpenGL Extension Loading Status: ERROR" << std::endl;
            exit(EXIT_FAILURE);
        }

        // TODO: add logger
        std::cout << "OpenGL Extension Loading Status: OK" << std::endl;
        std::cout << "OpenGL Info:\n";
        std::cout << "  Vendor: " << glGetString(GL_VENDOR) << '\n';
        std::cout << "  Renderer: " << glGetString(GL_RENDERER) << '\n';
        std::cout << "  Version: " << glGetString(GL_VERSION) << '\n';

        // TODO: setup config levels
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }
}