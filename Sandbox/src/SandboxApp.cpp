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
        modelVertexShader.Filepath = "assets/shaders/BlinnPhong.vert";
        modelVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs modelFragmentShader{};
        modelFragmentShader.Filepath = "assets/shaders/ModelShader.frag";
        modelFragmentShader.Filepath = "assets/shaders/BlinnPhong.frag";
        modelFragmentShader.Type = ShaderType::Fragment;
        m_ModelShader = new Shader({modelVertexShader, modelFragmentShader});

        PipelineState modelPipelineState{};
        m_ModelPipeline = new Pipeline(modelPipelineState, m_ModelShader);

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

        TextureDescription whiteTextureDesc = {};
        whiteTextureDesc.ImageExtent = { 1, 1, 1 };
        whiteTextureDesc.ImageFormat = ImageFormat::RGBA8;
        whiteTextureDesc.GenerateMipmaps = false;
        whiteTextureDesc.FilterMode = TextureFilterMode::Nearest;
        m_WhiteTexture = new Texture(whiteTextureDesc);
        m_WhiteTexture->SetData(&white);

        std::array<void*, 6> cubemapData = {
            checkerboardPixels.data(),
            checkerboardPixels.data(),
            checkerboardPixels.data(),
            checkerboardPixels.data(),
            checkerboardPixels.data(),
            checkerboardPixels.data(),
        };

        TextureDescription cubemapDesc = {};
        cubemapDesc.ImageType = ImageType::Cubemap;
        cubemapDesc.ImageExtent = { 16, 16, 1};
        cubemapDesc.FilterMode = TextureFilterMode::Nearest;
        cubemapDesc.MipmapMode = TextureMipmapFilterMode::LinearMipmap;
        cubemapDesc.GenerateMipmaps = true;
        m_Cubemap = new Texture(cubemapDesc);
        m_Cubemap->SetData(cubemapData.data());

        BufferDescription skyboxVertexDescription = {};
        skyboxVertexDescription.Type = BufferType::Vertex;
        skyboxVertexDescription.Size = g_SkyboxVertices.size() * sizeof(Vector3D);
        skyboxVertexDescription.Data = g_SkyboxVertices.data();
        m_SkyboxVertexBuffer = new Buffer(skyboxVertexDescription);

        m_SkyboxVertexAttribBinding = GetSkyboxVertexBinding();
        m_SkyboxVertexArray = new VertexArray({m_SkyboxVertexAttribBinding});

        BufferDescription skyboxIndexDescription = {};
        skyboxIndexDescription.Type = BufferType::Index;
        skyboxIndexDescription.Size = g_SkyboxIndices.size() * sizeof(uint32_t);
        skyboxIndexDescription.Data = g_SkyboxIndices.data();
        m_SkyboxIndexBuffer = new Buffer(skyboxIndexDescription);

        m_Xaxis = new VectorGizmo(Vector3D::zero, Vector3D::right, Color(1.0f, 0.0f, 0.0f, 1.0f));
        m_Yaxis = new VectorGizmo(Vector3D::zero, Vector3D::up, Color(0.0f, 1.0f, 0.0f, 1.0f));
        m_Zaxis = new VectorGizmo(Vector3D::zero, Vector3D::forward, Color(0.0f, 0.0f, 1.0f, 1.0f));
        m_Origin = new PointGizmo(Vector3D::zero, Color(0.6f, 0.6f, 0.6f, 1.0f));

        ShaderSpecs axisVertexShader{};
        axisVertexShader.Filepath = "assets/shaders/VectorGizmo.vert";
        axisVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs axisFragmentShader{};
        axisFragmentShader.Filepath = "assets/shaders/VectorGizmo.frag";
        axisFragmentShader.Type = ShaderType::Fragment;
        m_VectorGizmoShader = new Shader({axisVertexShader, axisFragmentShader});

        ShaderSpecs pointVertexShader{};
        pointVertexShader.Filepath = "assets/shaders/PointGizmo.vert";
        pointVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs pointFragmentShader{};
        pointFragmentShader.Filepath = "assets/shaders/PointGizmo.frag";
        pointFragmentShader.Type = ShaderType::Fragment;
        m_PointGizmoShader = new Shader({pointVertexShader, pointFragmentShader});

        // HACK: Should be two different pipelines
        PipelineState axisPipelineState{};
        m_AxisPipeline = new Pipeline(axisPipelineState, m_VectorGizmoShader);

        ShaderSpecs skyboxVertexShader{};
        skyboxVertexShader.Filepath = "assets/shaders/Skybox.vert";
        skyboxVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs skyboxFragmentShader{};
        skyboxFragmentShader.Filepath = "assets/shaders/Skybox.frag";
        skyboxFragmentShader.Type = ShaderType::Fragment;
        m_SkyboxShader = new Shader({skyboxVertexShader, skyboxFragmentShader});

        PipelineState skyboxPipelineState{};
        skyboxPipelineState.PolygonState.FrontFace = FrontFaceMode::Clockwise;
        skyboxPipelineState.DepthState.DepthFunc = DepthComparison::LessOrEqual;
        skyboxPipelineState.DepthState.DepthWrite = false;
        m_SkyboxPipeline = new Pipeline(skyboxPipelineState, m_SkyboxShader);
    }

    void Update() override {
        if (GetWindow()->ShouldClose())
            Quit();
    }

    void Render() override {
        RenderCommand::BeginFrame();

#define ROTATE_CAMERA 1
#if ROTATE_CAMERA
        static float angle = 0.0f;
        float speed = glm::radians(90.0f);
        Vector3D cameraPosition = Vector3D(0.5f * cos(angle) - 3.0f * sin(angle), 1.0f, 3.0f * cos(angle) + 0.5f * sin(angle));
        angle += speed * float(1.0 / 144.0);
#else
        Vector3D cameraPosition = Vector3D(0.5f, 1.0f, 3.0f);
#endif

        m_ModelPipeline->Bind();
        Matrix4x4 model = Matrix4x4::identity;
        model.Translate(Vector3D(0, 0, 0));
        model.Scale(Vector3D(.75f, .75f, .75f));
        Matrix4x4 view = Matrix4x4::LookAt(cameraPosition, Vector3D::zero, Vector3D::up);
#define LEFT_HANDED 0
#if LEFT_HANDED
        model.Scale(Vector3D(1, 1, -1));
        view.Scale(Vector3D(1, 1, -1));
        cameraPosition.z *= -1;
#endif
        // Logger::Info(model.Determinant());
        Matrix4x4 proj = Matrix4x4::Perspective(60.0f, 4.0f / 3.0f, 0.3f, 50.0f);
        Matrix4x4 modelViewProj = proj * view * model;

        m_ModelShader->SetMat4("u_ModelViewProj", modelViewProj);
        m_ModelShader->SetMat4("u_Model", model);
        m_ModelShader->SetMat4("u_Normal", model.Inverse().Transpose());
        m_ModelShader->SetFloat4("u_LightDir", Vector4D(1.0f, 1.0f, 1.0f, 0.0f));
        m_ModelShader->SetFloat3("u_CameraPosition", cameraPosition);

        m_ModelShader->SetColor("u_MainColor", Color(0.9f, 0.1f, 0.1f, 1.0f));
        m_ModelShader->SetColor("u_Material.diffuseColor", Color(0.9f, 0.1f, 0.1f, 1.0f));
        m_ModelShader->SetColor("u_Material.specularColor", Color(1.0f, 1.0f, 1.0f, 1.0f));
        m_ModelShader->SetColor("u_Material.emissiveColor", Color(0.0f, 0.0f, 0.0f, 1.0f));

        m_WhiteTexture->BindTextureUnit(0);
        m_WhiteTexture->BindTextureUnit(1);
        m_WhiteTexture->BindTextureUnit(2);
        // m_CheckerboardTexture->BindTextureUnit(0);

        for (const Mesh& mesh : m_SphereModel->GetMeshes())
            mesh.Render();

        m_TrianglePipeline->Bind();
        RenderCommand::BindVertexArray(m_TriangleVertexArray);
        RenderCommand::SetVertexBuffer(m_VertexBuffer, m_TriangleVertexAttribBinding);
        // RenderCommand::Draw(3);

        // TODO: extract into a separated engine/editor feature
        {
            RenderCommand::SetViewport(0, 500, 100, 100);
            m_AxisPipeline->Bind();

            Matrix4x4 vectorModel = Matrix4x4::identity;
#if LEFT_HANDED
            vectorModel.columns[2].z *= -1;
            cameraPosition.z *= -1;
#endif
            Matrix4x4 vectorView = Matrix4x4::LookAt(cameraPosition.Normalized(), Vector3D::zero, Vector3D::up);
            Matrix4x4 vectorProj = Matrix4x4::Ortho(-1.25f, 1.25f, -1.25f, 1.25f, 0.03f, 2.0f);

            m_VectorGizmoShader->SetMat4("u_ModelViewProj", vectorProj * vectorView * vectorModel);
            m_Xaxis->Render();
            m_Yaxis->Render();
            m_Zaxis->Render();
            RenderCommand::UseShader(m_PointGizmoShader);
            m_PointGizmoShader->SetMat4("u_ModelViewProj", vectorProj * vectorView * vectorModel);
            m_Origin->Render();

            RenderCommand::SetViewport(0, 0, 800, 600);
        }

        Matrix4x4 skyboxView = view;
        skyboxView.columns[3] = Vector4D::zero;
        skyboxView.columns[3].w = 1.0f;
        Matrix4x4 skyboxViewProj = proj * skyboxView;

        m_SkyboxShader->SetMat4("u_ViewProj", skyboxViewProj);
        m_SkyboxPipeline->Bind();
        m_Cubemap->BindTextureUnit(0);

        RenderCommand::BindVertexArray(m_SkyboxVertexArray);
        RenderCommand::SetVertexBuffer(m_SkyboxVertexBuffer, m_SkyboxVertexAttribBinding);
        RenderCommand::SetIndexBuffer(m_SkyboxIndexBuffer);
        RenderCommand::DrawIndexed(36);

        RenderCommand::EndFrame();
        GetWindow()->SwapBuffers();
    }

    void Cleanup() override {
        Logger::Trace("Resource cleanup...");
        delete m_TriangleVertexArray;
        delete m_VertexBuffer;

        delete m_SkyboxPipeline;
        delete m_SkyboxShader;
        delete m_SkyboxVertexArray;
        delete m_SkyboxVertexBuffer;
        delete m_SkyboxIndexBuffer;

        delete m_Xaxis;
        delete m_Yaxis;
        delete m_Zaxis;

        delete m_AxisPipeline;
        delete m_VectorGizmoShader;
        delete m_PointGizmoShader;

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

    Buffer* m_SkyboxVertexBuffer = nullptr;
    Buffer* m_SkyboxIndexBuffer = nullptr;
    VertexArray* m_SkyboxVertexArray = nullptr;
    VertexAttribBinding m_SkyboxVertexAttribBinding = {};

    Model* m_SphereModel = nullptr;

    Shader* m_TriangleShader = nullptr;
    Shader* m_ModelShader = nullptr;
    Shader* m_VectorGizmoShader = nullptr;
    Shader* m_PointGizmoShader = nullptr;
    Shader* m_SkyboxShader = nullptr;

    Pipeline* m_TrianglePipeline = nullptr;
    Pipeline* m_ModelPipeline = nullptr;
    Pipeline* m_AxisPipeline = nullptr;
    Pipeline* m_SkyboxPipeline = nullptr;

    Texture* m_CheckerboardTexture = nullptr;
    Texture* m_WhiteTexture = nullptr;
    Texture* m_Cubemap = nullptr;

    VectorGizmo* m_Xaxis = nullptr;
    VectorGizmo* m_Yaxis = nullptr;
    VectorGizmo* m_Zaxis = nullptr;
    PointGizmo* m_Origin = nullptr;
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
