#define OG_ENTRY_POINT
#include <OpenEngine.h>

using namespace OpenGraphics;
using Random = UniformRandom;

#define ROTATE_CAMERA 0

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

        TextureDescription checkerTextureDesc = {};
        checkerTextureDesc.ImageExtent = { 16, 16, 1 };
        checkerTextureDesc.ImageFormat = ImageFormat::RGBA8;
        checkerTextureDesc.GenerateMipmaps = false;
        checkerTextureDesc.FilterMode = TextureFilterMode::Nearest;
        m_CheckerboardTexture = new Texture(checkerTextureDesc);

        const uint32_t gray = 0x88888888;
        const uint32_t white = 0xffffffff;
        const uint32_t black = 0x00000000;
        const uint32_t magenta = 0xffff00ff;
        std::array<uint32_t, 16 * 16> checkerboardPixels = { 0 };
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                checkerboardPixels[x * 16 + y] = ((x % 2) ^ (y % 2)) ? magenta : black;
            }
        }
        m_CheckerboardTexture->SetData(checkerboardPixels.data());

        m_RenderCamera = new RenderCamera(Matrix4x4::Perspective(60.0f, 4.0f / 3.0f, 0.3f, 50.0f));
        m_RenderCamera->SetPosition(Vector3D(0.5f, 1.0f, 3.0f));
        m_RenderCamera->SetView(Matrix4x4::LookAt(m_RenderCamera->GetPosition(), Vector3D::zero, Vector3D::up));
    }

    void Update() override {
        if (GetWindow()->ShouldClose())
            Quit();

#if ROTATE_CAMERA
        static float angle = 0.0f;
        float speed = 1.5f;
        m_RenderCamera->SetPosition(
            Vector3D(
                0.5f * cos(angle) - 3.0f * sin(angle),
                1.0f,
                3.0f * cos(angle) + 0.5f * sin(angle)
            ));
        angle += speed * float(1.0 / 144.0);
#endif

        m_RenderCamera->SetView(Matrix4x4::LookAt(m_RenderCamera->GetPosition(), Vector3D::zero, Vector3D::up));
    }

    void Render() override {
        RenderSystem::BeginFrame();
        RenderSystem::RenderScene({m_RenderCamera});

        m_TrianglePipeline->Bind();
        RenderCommand::BindVertexArray(m_TriangleVertexArray);
        RenderCommand::SetVertexBuffer(m_VertexBuffer, m_TriangleVertexAttribBinding);
        RenderCommand::Draw(3);

        RenderSystem::EndFrame();
        GetWindow()->SwapBuffers();
    }

    void Cleanup() override {
        Logger::Trace("Resource cleanup...");
        delete m_RenderCamera;

        delete m_TriangleVertexArray;
        delete m_VertexBuffer;

        delete m_CheckerboardTexture;
        delete m_TrianglePipeline;

        delete m_TriangleShader;
    }

private:
    Buffer* m_VertexBuffer = nullptr;
    VertexArray* m_TriangleVertexArray = nullptr;
    VertexAttribBinding m_TriangleVertexAttribBinding = {};

    Shader* m_TriangleShader = nullptr;
    Pipeline* m_TrianglePipeline = nullptr;

    Texture* m_CheckerboardTexture = nullptr;

    RenderCamera* m_RenderCamera = nullptr;
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
