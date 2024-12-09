#define OG_ENTRY_POINT
#include <OpenEngine.h>

using namespace OpenGraphics;
using Random = UniformRandom;

class SandboxApp : public OpenGraphics::Application
{
public:
    SandboxApp() = default;
    ~SandboxApp() = default;

    void Load() override {
        Logger::Trace("Resource loading...");

        float vertexBuffer[] = {
            0.0f,  0.5f, 1.0f, 0.0f, 0.0f,
           -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        };

        Random rng = Random();
        Vector3D vector = Vector3D(rng.NextFloat(), rng.NextFloat(), rng.NextFloat());
        Logger::Trace(vector);

        Logger::Trace(rng.Range(1, 6));
        Logger::Trace(rng.Range(1, 50));

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
        RenderCommand::BindVertexArray(m_TriangleVertexArray);
        RenderCommand::SetVertexBuffer(m_VertexBuffer, m_TriangleVertexAttribBinding);
        RenderCommand::Draw(3);
        RenderCommand::EndFrame();
        GetWindow()->SwapBuffers();
    }

    void Cleanup() override {
        Logger::Trace("Resource cleanup...");
        delete m_TriangleVertexArray;
        delete m_VertexBuffer;
    }

private:
    Buffer* m_VertexBuffer = nullptr;
    VertexArray* m_TriangleVertexArray = nullptr;
    VertexAttribBinding m_TriangleVertexAttribBinding = {};
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
