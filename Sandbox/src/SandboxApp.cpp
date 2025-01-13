#define OG_ENTRY_POINT
#define GLM_FORCE_LEFT_HANDED
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
            0.0f,  0.35f, 1.0f, 0.0f, 0.0f,
           -0.3f, -0.25f, 0.0f, 1.0f, 0.0f,
            0.3f, -0.25f, 0.0f, 0.0f, 1.0f,
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

        ShaderSpecs triangleVertexShader{};
        triangleVertexShader.Filepath = "assets/shaders/TriangleShader.vert";
        triangleVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs triangleFragmentShader{};
        triangleFragmentShader.Filepath = "assets/shaders/TriangleShader.frag";
        triangleFragmentShader.Type = ShaderType::Fragment;
        m_TriangleShader = new Shader({triangleVertexShader, triangleFragmentShader});

        m_SphereModel = new Model("assets/models/Sphere.obj");

        ShaderSpecs modelVertexShader{};
        modelVertexShader.Filepath = "assets/shaders/ModelShader.vert";
        modelVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs modelFragmentShader{};
        modelFragmentShader.Filepath = "assets/shaders/ModelShader.frag";
        modelFragmentShader.Type = ShaderType::Fragment;
        m_ModelShader = new Shader({modelVertexShader, modelFragmentShader});
    }

    void Update() override {
        if (GetWindow()->ShouldClose())
            Quit();
    }

    void Render() override {
        RenderCommand::BeginFrame();

        RenderCommand::UseShader(m_ModelShader);
        Matrix4x4 model = Matrix4x4::identity;
        model.Translate(Vector3D(0, 0, -3));
        model.Scale(Vector3D(.75f, .75f, .75f));
        Matrix4x4 view = Matrix4x4::LookAt(Vector3D(0.0f, 0.0f, -5.0f), Vector3D::zero, Vector3D::up);
        Matrix4x4 proj = Matrix4x4::Perspective(60.0f, 4.0f / 3.0f, 0.3f, 50.0f);
        Matrix4x4 modelViewProj = proj * view * model;

        m_ModelShader->SetMat4("u_ModelViewProj", modelViewProj);
        m_ModelShader->SetMat4("u_Model", model);
        m_ModelShader->SetMat4("u_Normal", model.Inverse().Transpose());
        m_ModelShader->SetFloat4("u_LightDir", Vector4D(1.0f, 1.0f, -1.0f, 0.0f));

        for (const Mesh& mesh : m_SphereModel->GetMeshes())
            mesh.Render();

        RenderCommand::UseShader(m_TriangleShader);
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

        delete m_SphereModel;

        delete m_TriangleShader;
        delete m_ModelShader;
    }

private:
    Buffer* m_VertexBuffer = nullptr;
    VertexArray* m_TriangleVertexArray = nullptr;
    VertexAttribBinding m_TriangleVertexAttribBinding = {};

    Model* m_SphereModel = nullptr;

    Shader* m_TriangleShader = nullptr;
    Shader* m_ModelShader = nullptr;
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
