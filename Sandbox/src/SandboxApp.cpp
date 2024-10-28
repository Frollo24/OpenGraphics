#define OG_ENTRY_POINT
#include <OpenEngine.h>
#include <iostream>

using namespace OpenGraphics;

class SandboxApp : public OpenGraphics::Application
{
public:
    SandboxApp() = default;
    ~SandboxApp() = default;

    void Load() override {
        std::cout << "Resource loading..." << std::endl;

        float vertexBuffer[] = {
            0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
           -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        };

        BufferDescription vertexDescription = {};
        vertexDescription.Type = BufferType::Vertex;
        vertexDescription.Size = sizeof(vertexBuffer);
        vertexDescription.Data = vertexBuffer;
        m_VertexBuffer = new Buffer(vertexDescription);

        m_TriangleVertexAttribBinding = {
            {ShaderDataType::Float2, "aPosition"},
            {ShaderDataType::Float3, "aColor"},
        };

        m_TriangleVertexArray = new VertexArray({m_TriangleVertexAttribBinding});
    }

    void Update() override {
        if (GetWindow()->ShouldClose())
            Quit();
    }

    void Render() override {
        RenderCommand::BeginFrame();
        RenderCommand::BindVertexBuffer(m_TriangleVertexArray, m_VertexBuffer, m_TriangleVertexAttribBinding);
        RenderCommand::Draw(3);
        RenderCommand::EndFrame();
        GetWindow()->SwapBuffers();
    }

    void Cleanup() override {
        std::cout << "Resource cleanup..." << std::endl;
    }

private:
    Buffer* m_VertexBuffer = nullptr;
    VertexArray* m_TriangleVertexArray = nullptr;
    VertexAttribBinding m_TriangleVertexAttribBinding = {};
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
