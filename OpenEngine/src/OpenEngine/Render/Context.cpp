#include "ogpch.h"
#include "Context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace OpenGraphics
{
    static void OpenGLDebugCallback(GLenum source, GLenum type, unsigned id, GLenum severity, int length, const char* message, const void* userParam)
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         Logger::Error("OpenGL: {}", message); return;
            case GL_DEBUG_SEVERITY_MEDIUM:       Logger::Warn("OpenGL: {}", message); return;
            case GL_DEBUG_SEVERITY_LOW:          Logger::Info("OpenGL: {}", message); return;
            case GL_DEBUG_SEVERITY_NOTIFICATION: Logger::Trace("OpenGL: {}", message); return;
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
            Logger::Fatal("OpenGL Extension Loading Status: ERROR");
            exit(EXIT_FAILURE);
        }

        Logger::Info("OpenGL Extension Loading Status: OK");
        Logger::Debug("OpenGL Info:");
        Logger::Debug("  Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        Logger::Debug("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        Logger::Debug("  Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

        // TODO: setup config levels
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }
}