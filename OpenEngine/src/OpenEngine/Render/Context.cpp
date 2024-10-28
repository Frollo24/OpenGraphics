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