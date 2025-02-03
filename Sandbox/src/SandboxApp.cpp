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

        PipelineState trianglePipelineState{};
        m_TrianglePipeline = new Pipeline(trianglePipelineState, m_TriangleShader);

        m_SphereModel = new Model("assets/models/Sphere.obj");

        ShaderSpecs modelVertexShader{};
        modelVertexShader.Filepath = "assets/shaders/ModelShader.vert";
        modelVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs modelFragmentShader{};
        modelFragmentShader.Filepath = "assets/shaders/ModelShader.frag";
        modelFragmentShader.Type = ShaderType::Fragment;
        m_ModelShader = new Shader({modelVertexShader, modelFragmentShader});

        PipelineState modelPipelineState{};
        // TODO: check handedness of the models
        modelPipelineState.PolygonState.FrontFace = FrontFaceMode::Clockwise;
        m_ModelPipeline = new Pipeline(modelPipelineState, m_ModelShader);

        TextureDescription textureDesc = {};
        textureDesc.ImageExtent = { 16, 16, 1 };
        textureDesc.ImageFormat = ImageFormat::RGBA8;
        textureDesc.GenerateMipmaps = false;
        textureDesc.FilterMode = TextureFilterMode::Nearest;
        m_CheckerboardTexture = new Texture(textureDesc);

        const uint32_t gray = 0x88888888;
        const uint32_t white = 0xffffffff;
        const uint32_t black = 0x00000000;
        const uint32_t magenta = 0xffff00ff;
        std::array<uint32_t, 16 * 16 > checkerboardPixels = { 0 };
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                checkerboardPixels[x * 16 + y] = ((x % 2) ^ (y % 2)) ? magenta : black;
            }
        }
        m_CheckerboardTexture->SetData(checkerboardPixels.data());
    }

    void Update() override {
        if (GetWindow()->ShouldClose())
            Quit();
    }

    void Render() override {
        RenderCommand::BeginFrame();

        m_ModelPipeline->Bind();
        Matrix4x4 model = Matrix4x4::identity;
        model.Translate(Vector3D(0, 0, -3));
        model.Scale(Vector3D(.75f, .75f, .75f));
        Matrix4x4 view = Matrix4x4::LookAt(Vector3D(0.0f, 0.0f, -5.0f), Vector3D::zero, Vector3D::up);
        Matrix4x4 proj = Matrix4x4::Perspective(60.0f, 4.0f / 3.0f, 0.3f, 50.0f);
        Matrix4x4 modelViewProj = proj * view * model;

        m_ModelShader->SetMat4("u_ModelViewProj", modelViewProj);
        m_ModelShader->SetMat4("u_Model", model);
        m_ModelShader->SetMat4("u_Normal", model.Inverse().Transpose());
        m_ModelShader->SetFloat4("u_LightDir", Vector4D(1.0f, 1.0f, 1.0f, 0.0f));

        m_CheckerboardTexture->BindTextureUnit(0);

        for (const Mesh& mesh : m_SphereModel->GetMeshes())
            mesh.Render();

        m_TrianglePipeline->Bind();
        RenderCommand::BindVertexArray(m_TriangleVertexArray);
        RenderCommand::SetVertexBuffer(m_VertexBuffer, m_TriangleVertexAttribBinding);
        // RenderCommand::Draw(3);

        RenderCommand::EndFrame();
        GetWindow()->SwapBuffers();
    }

    void Cleanup() override {
        Logger::Trace("Resource cleanup...");
        delete m_TriangleVertexArray;
        delete m_VertexBuffer;

        delete m_SphereModel;

        delete m_CheckerboardTexture;

        delete m_ModelPipeline;
        delete m_TrianglePipeline;

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

    Pipeline* m_TrianglePipeline = nullptr;
    Pipeline* m_ModelPipeline = nullptr;

    Texture* m_CheckerboardTexture = nullptr;
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
