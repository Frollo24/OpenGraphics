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
        auto triangleShader = CreateRef<Shader>({triangleVertexShader, triangleFragmentShader});

        PipelineState trianglePipelineState{};
        m_TrianglePipeline = new Pipeline(trianglePipelineState, triangleShader);

        m_EditorCamera = new EditorCamera(Matrix4x4::Perspective(60.0f, 4.0f / 3.0f, 0.3f, 50.0f));
        m_EditorCamera->SetPosition(Vector3D(0.5f, 1.0f, 3.0f));
        m_EditorCamera->SetView(Matrix4x4::LookAt(m_EditorCamera->GetPosition(), Vector3D::zero, Vector3D::up));

        m_Scene = new Scene();

        m_SphereGameObject = new GameObject();
        m_SphereGameObject->GetTransform()->Scale(Vector3D(.75f, .75f, .75f));
        const auto starGameObject = new GameObject(Vector3D(0, 0, 2), m_SphereGameObject->GetTransform());
        starGameObject->GetTransform()->Rotate(Vector3D::left, 15);

        // HACK: materials should be modifiable by changing the reference to the material
        const auto sphereRenderComponent = m_SphereGameObject->AddComponent<RenderComponent>();
        Model* sphereModel = new Model("assets/models/Sphere.obj");

        auto& sphereMaterial = const_cast<Material&>(sphereModel->GetMeshes().at(0).GetMaterial());
        sphereMaterial.MainColor = Color(0.9f, 0.1f, 0.1f, 1.0f);
        sphereMaterial.SetColor("_SpecularColor", Color::yellow);
        sphereMaterial.SetColor("_EmissiveColor", Color::black);
        sphereMaterial.SetFloat("_Metallic", 0.0f);
        sphereMaterial.SetFloat("_Roughness", 0.25f);
        Logger::Debug("{} shininess: {}", sphereMaterial.Name, sphereMaterial.GetFloat("_Shininess"));

        sphereRenderComponent->SetModel(sphereModel);
        const auto starRenderComponent = starGameObject->AddComponent<RenderComponent>();
        Model* starModel = new Model("assets/models/Star.obj");
        auto& starMaterial = const_cast<Material&>(starModel->GetMeshes().at(0).GetMaterial());
        starMaterial.MainColor = Color(0.8f, 0.65f, 0.0f, 1.0f);
        starMaterial.SetFloat("_Metallic", 0.85f);
        starMaterial.SetFloat("_Roughness", 0.25f);
        starRenderComponent->SetModel(starModel);

        m_Scene->AddGameObject(m_SphereGameObject);
        m_Scene->AddGameObject(starGameObject);

        m_SceneRenderer = SceneRenderer(m_Scene);
        m_SceneRenderer.SetEditorCamera(m_EditorCamera);

        TextureDescription textureDesc = {};
        auto refArray = RefArray<Texture, 5>();
        const auto ref = refArray.CreateRef(rng.Range(0, 4), textureDesc);
        Logger::Info(ref->GetRendererID());

        auto dynamicRefArray = DynamicRefArray<Texture>(1);
        dynamicRefArray.EmplaceNew(textureDesc);
        auto dynRef1 = dynamicRefArray.CreateRefAt(0);
        dynamicRefArray.EmplaceNew(textureDesc);
        auto dynRef2 = dynamicRefArray.CreateRefAt(0);

        OG_ASSERT(dynRef1.Get() == dynRef2.Get(), "Bad pointer sync");
    }

    void Update() override {
        if (GetWindow()->ShouldClose())
            Quit();

        m_SphereGameObject->GetTransform()->Rotate(Vector3D::up, 1.0f);
        m_Scene->OnUpdate();

#if ROTATE_CAMERA
        static float angle = 0.0f;
        float speed = 1.5f;
        m_EditorCamera->SetPosition(
            Vector3D(
                0.5f * cos(angle) - 3.0f * sin(angle),
                1.0f,
                3.0f * cos(angle) + 0.5f * sin(angle)
            ));
        angle += speed * float(1.0 / 144.0);
#endif

        m_EditorCamera->SetView(Matrix4x4::LookAt(m_EditorCamera->GetPosition(), Vector3D::zero, Vector3D::up));
    }

    void Render() override {
        RenderSystem::BeginFrame();

        // NOTE: on a proper editor application, we can use two similar scene renderers which only differs on
        // having an aditional editor camera
        m_SceneRenderer.BeginRendering();
        RenderSystem::RenderScene(m_SceneRenderer);
        m_SceneRenderer.EndRendering();

        m_TrianglePipeline->Bind();
        RenderCommand::BindVertexArray(m_TriangleVertexArray);
        RenderCommand::SetVertexBuffer(m_VertexBuffer, m_TriangleVertexAttribBinding);
        RenderCommand::SetPrimitiveTopology(PrimitiveTopology::Triangles);
        // RenderCommand::Draw(0, 3);

        RenderSystem::EndFrame();
        GetWindow()->SwapBuffers();
    }

    void Cleanup() override {
        Logger::Trace("Resource cleanup...");
        delete m_EditorCamera;
        delete m_Scene;

        delete m_TriangleVertexArray;
        delete m_VertexBuffer;

        delete m_TrianglePipeline;
    }

private:
    Buffer* m_VertexBuffer = nullptr;
    VertexArray* m_TriangleVertexArray = nullptr;
    VertexAttribBinding m_TriangleVertexAttribBinding = {};

    Pipeline* m_TrianglePipeline = nullptr;

    EditorCamera* m_EditorCamera = nullptr;
    Scene* m_Scene = nullptr;
    GameObject* m_SphereGameObject = nullptr;

    SceneRenderer m_SceneRenderer = nullptr;
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
