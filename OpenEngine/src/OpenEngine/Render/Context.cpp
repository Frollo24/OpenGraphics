#include "Context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenEngine/Core/Logger.h"

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

        // HACK: we should create a proper pipeline and a proper shader before drawing anything
        const GLchar* vsGLSL = R"(#version 450
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

layout(location = 0) out vec3 vColor;

void main(){
	gl_Position = vec4(aPosition, 0.0, 1.0);
	vColor = aColor;
}
)";
        const GLchar* fsGLSL = R"(#version 450
layout(location = 0) in vec3 vColor;

layout(location = 0) out vec4 oColor;

void main(){
	oColor = vec4(vColor, 1.0);
}
)";

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsGLSL, nullptr);
        glCompileShader(vs);
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fsGLSL, nullptr);
        glCompileShader(fs);
        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glUseProgram(program);
    }
}