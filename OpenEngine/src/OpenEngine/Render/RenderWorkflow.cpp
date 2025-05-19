#include "ogpch.h"
#include "RenderWorkflow.h"
#include "RenderCommand.h"

// HACK: this shouldn't be directly included here
#include "RenderStructs.h"
#include "OpenEngine/Asset/TextureImporter.h"
#include "OpenEngine/Elements/VectorGizmo.h"
#include "OpenEngine/Elements/PointGizmo.h"
#include "OpenEngine/Scene/Scene.h"
#include "OpenEngine/Scene/RenderComponent.h"

// NOTE: could be interesting to leave this as a parameter for the user
#define LEFT_HANDED 0

namespace OpenGraphics
{
    // GameObjects
    static Shader* s_ModelShader = nullptr;
    static Pipeline* s_ModelPipeline = nullptr;
    static Texture* s_WhiteTexture = nullptr;

    // Gizmos
    static VectorGizmo* s_Xaxis = nullptr;
    static VectorGizmo* s_Yaxis = nullptr;
    static VectorGizmo* s_Zaxis = nullptr;
    static PointGizmo* s_Origin = nullptr;
    static Shader* s_VectorGizmoShader = nullptr;
    static Shader* s_PointGizmoShader = nullptr;
    static Pipeline* s_AxisPipeline = nullptr;

    // Skybox
    static Buffer* s_SkyboxVertexBuffer = nullptr;
    static Buffer* s_SkyboxIndexBuffer = nullptr;
    static VertexArray* s_SkyboxVertexArray = nullptr;
    static VertexAttribBinding s_SkyboxVertexAttribBinding = {};
    static Shader* s_SkyboxShader = nullptr;
    static Pipeline* s_SkyboxPipeline = nullptr;
    static Texture* s_SkyboxCubemap = nullptr;

    void RenderWorkflow::Render(const std::vector<const RenderCamera*>& cameras)
    {
        for (const auto& camera : cameras)
        {
            DrawGameObjects(camera);
            DrawSkybox(camera);
            DrawGizmos(camera);
        }
    }

    void RenderWorkflow::InitResources()
    {
#pragma region GameObjects
        ShaderSpecs modelVertexShader{};
        modelVertexShader.Filepath = "assets/shaders/ModelShader.vert";
        modelVertexShader.Filepath = "assets/shaders/BlinnPhong.vert";
        modelVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs modelFragmentShader{};
        modelFragmentShader.Filepath = "assets/shaders/ModelShader.frag";
        modelFragmentShader.Filepath = "assets/shaders/BlinnPhong.frag";
        modelFragmentShader.Type = ShaderType::Fragment;
        s_ModelShader = new Shader({modelVertexShader, modelFragmentShader});

        PipelineState modelPipelineState{};
        s_ModelPipeline = new Pipeline(modelPipelineState, s_ModelShader);

        TextureDescription whiteTextureDesc = {};
        whiteTextureDesc.ImageExtent = { 1, 1, 1 };
        whiteTextureDesc.ImageFormat = ImageFormat::RGBA8;
        whiteTextureDesc.GenerateMipmaps = false;
        whiteTextureDesc.FilterMode = TextureFilterMode::Nearest;
        s_WhiteTexture = new Texture(whiteTextureDesc);

        const uint32_t white = 0xffffffff;
        s_WhiteTexture->SetData(&white);
#pragma endregion

#pragma region Skybox
        // Skybox
        BufferDescription skyboxVertexDescription = {};
        skyboxVertexDescription.Type = BufferType::Vertex;
        skyboxVertexDescription.Size = g_SkyboxVertices.size() * sizeof(Vector3D);
        skyboxVertexDescription.Data = g_SkyboxVertices.data();
        s_SkyboxVertexBuffer = new Buffer(skyboxVertexDescription);

        s_SkyboxVertexAttribBinding = GetSkyboxVertexBinding();
        s_SkyboxVertexArray = new VertexArray({s_SkyboxVertexAttribBinding});

        BufferDescription skyboxIndexDescription = {};
        skyboxIndexDescription.Type = BufferType::Index;
        skyboxIndexDescription.Size = g_SkyboxIndices.size() * sizeof(uint32_t);
        skyboxIndexDescription.Data = g_SkyboxIndices.data();
        s_SkyboxIndexBuffer = new Buffer(skyboxIndexDescription);

        auto cubemapFaces = TextureImporter::LoadCubemapFromSeparateFaces("assets/textures/skyboxFaces/",
            { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" }
        );

        std::array<void*, 6> cubemapData = {nullptr};
        for (int i = 0; i < cubemapFaces.size(); i++)
            cubemapData[i] = cubemapFaces[i].Data;

        TextureDescription cubemapDesc = {};
        cubemapDesc.ImageType = ImageType::Cubemap;
        cubemapDesc.ImageExtent = { (uint32_t)cubemapFaces[0].Width, (uint32_t)cubemapFaces[0].Height, 1};
        cubemapDesc.ImageFormat = ImageFormat::RGB8;
        cubemapDesc.FilterMode = TextureFilterMode::Nearest;
        cubemapDesc.MipmapMode = TextureMipmapFilterMode::LinearMipmap;
        cubemapDesc.GenerateMipmaps = true;
        s_SkyboxCubemap = new Texture(cubemapDesc);
        s_SkyboxCubemap->SetData(cubemapData.data());

        ShaderSpecs skyboxVertexShader{};
        skyboxVertexShader.Filepath = "assets/shaders/Skybox.vert";
        skyboxVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs skyboxFragmentShader{};
        skyboxFragmentShader.Filepath = "assets/shaders/Skybox.frag";
        skyboxFragmentShader.Type = ShaderType::Fragment;
        s_SkyboxShader = new Shader({skyboxVertexShader, skyboxFragmentShader});

        PipelineState skyboxPipelineState{};
#if !LEFT_HANDED
        skyboxPipelineState.PolygonState.FrontFace = FrontFaceMode::Clockwise;
#endif
        skyboxPipelineState.DepthState.DepthFunc = DepthComparison::LessOrEqual;
        skyboxPipelineState.DepthState.DepthWrite = false;
        s_SkyboxPipeline = new Pipeline(skyboxPipelineState, s_SkyboxShader);
#pragma endregion

#pragma region Gizmos
        s_Xaxis = new VectorGizmo(Vector3D::zero, Vector3D::right, Color(1.0f, 0.0f, 0.0f, 1.0f));
        s_Yaxis = new VectorGizmo(Vector3D::zero, Vector3D::up, Color(0.0f, 1.0f, 0.0f, 1.0f));
        s_Zaxis = new VectorGizmo(Vector3D::zero, Vector3D::forward, Color(0.0f, 0.0f, 1.0f, 1.0f));
        s_Origin = new PointGizmo(Vector3D::zero, Color(0.6f, 0.6f, 0.6f, 1.0f));

        ShaderSpecs axisVertexShader{};
        axisVertexShader.Filepath = "assets/shaders/VectorGizmo.vert";
        axisVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs axisFragmentShader{};
        axisFragmentShader.Filepath = "assets/shaders/VectorGizmo.frag";
        axisFragmentShader.Type = ShaderType::Fragment;
        s_VectorGizmoShader = new Shader({axisVertexShader, axisFragmentShader});

        ShaderSpecs pointVertexShader{};
        pointVertexShader.Filepath = "assets/shaders/PointGizmo.vert";
        pointVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs pointFragmentShader{};
        pointFragmentShader.Filepath = "assets/shaders/PointGizmo.frag";
        pointFragmentShader.Type = ShaderType::Fragment;
        s_PointGizmoShader = new Shader({pointVertexShader, pointFragmentShader});

        // HACK: Should be two different pipelines
        PipelineState axisPipelineState{};
        s_AxisPipeline = new Pipeline(axisPipelineState, s_VectorGizmoShader);
#pragma endregion
    }

    void RenderWorkflow::DeleteResources()
    {
#pragma region GameObjects
        delete s_ModelShader;
        delete s_ModelPipeline;
        delete s_WhiteTexture;
#pragma endregion

#pragma region Skybox
        delete s_SkyboxVertexArray;
        delete s_SkyboxVertexBuffer;
        delete s_SkyboxIndexBuffer;
        delete s_SkyboxCubemap;
        delete s_SkyboxShader;
        delete s_SkyboxPipeline;
#pragma endregion

#pragma region Gizmos
        delete s_Xaxis;
        delete s_Yaxis;
        delete s_Zaxis;
        delete s_Origin;

        delete s_AxisPipeline;
        delete s_VectorGizmoShader;
        delete s_PointGizmoShader;
#pragma endregion
    }

    void RenderWorkflow::DrawGameObjects(const RenderCamera* camera)
    {
        Vector3D cameraPosition = camera->GetPosition();

        s_ModelPipeline->Bind();
        auto sphereGameObject = camera->GetScene()->GetGameObjects().at(0);
        Matrix4x4 model = sphereGameObject->GetTransform()->GetModelMatrix();
        Matrix4x4 view = camera->GetView();

#if LEFT_HANDED
        model.Scale(Vector3D(1, 1, -1));
        view.Scale(Vector3D(1, 1, -1));
        cameraPosition.z *= -1;
#endif
        Matrix4x4 proj = camera->GetProjection();
        Matrix4x4 modelViewProj = proj * view * model;

        s_ModelShader->SetFloat4("u_LightDir", Vector4D(1.0f, 1.0f, 1.0f, 0.0f));
        s_ModelShader->SetFloat3("u_CameraPosition", cameraPosition);

        s_ModelShader->SetMat4("u_ModelViewProj", modelViewProj);
        s_ModelShader->SetMat4("u_Model", model);
        s_ModelShader->SetMat4("u_Normal", model.Inverse().Transpose());
        s_ModelShader->SetColor("u_MainColor", Color(0.9f, 0.1f, 0.1f, 1.0f));
        s_ModelShader->SetColor("u_Material.diffuseColor", Color(0.9f, 0.1f, 0.1f, 1.0f));
        s_ModelShader->SetColor("u_Material.specularColor", Color(1.0f, 1.0f, 1.0f, 1.0f));
        s_ModelShader->SetColor("u_Material.emissiveColor", Color(0.0f, 0.0f, 0.0f, 1.0f));

        s_WhiteTexture->BindTextureUnit(0);
        s_WhiteTexture->BindTextureUnit(1);
        s_WhiteTexture->BindTextureUnit(2);
        sphereGameObject->OnRender();

        auto starGameObject = camera->GetScene()->GetGameObjects().at(1);
        model = starGameObject->GetTransform()->GetModelMatrix();

#if LEFT_HANDED
        model.Scale(Vector3D(1, 1, -1));
#endif

        modelViewProj = proj * view * model;
        s_ModelShader->SetMat4("u_ModelViewProj", modelViewProj);
        s_ModelShader->SetMat4("u_Model", model);
        s_ModelShader->SetMat4("u_Normal", model.Inverse().Transpose());
        s_ModelShader->SetColor("u_MainColor", Color(0.8f, 0.65f, 0.0f, 1.0f));
        s_ModelShader->SetColor("u_Material.diffuseColor", Color(0.8f, 0.65f, 0.0f, 1.0f));
        s_ModelShader->SetColor("u_Material.specularColor", Color(1.0f, 1.0f, 1.0f, 1.0f));
        s_ModelShader->SetColor("u_Material.emissiveColor", Color(0.0f, 0.0f, 0.0f, 1.0f));

        starGameObject->OnRender();
    }

    void RenderWorkflow::DrawSkybox(const RenderCamera* camera)
    {
        Matrix4x4 skyboxView = camera->GetView();
        const Matrix4x4& skyboxProj = camera->GetProjection();

#if LEFT_HANDED
        skyboxView.Scale(Vector3D(1, 1, -1));
#endif

        skyboxView.columns[3] = Vector4D::zero;
        skyboxView.columns[3].w = 1.0f;
        const Matrix4x4 skyboxViewProj = skyboxProj * skyboxView;

        s_SkyboxShader->SetMat4("u_ViewProj", skyboxViewProj);
        s_SkyboxPipeline->Bind();
        s_SkyboxCubemap->BindTextureUnit(0);

        RenderCommand::BindVertexArray(s_SkyboxVertexArray);
        RenderCommand::SetVertexBuffer(s_SkyboxVertexBuffer, s_SkyboxVertexAttribBinding);
        RenderCommand::SetIndexBuffer(s_SkyboxIndexBuffer);
        RenderCommand::DrawIndexed(36);
    }

    void RenderWorkflow::DrawGizmos(const RenderCamera* camera)
    {
        Vector3D cameraPosition = camera->GetPosition();

        RenderCommand::SetViewport(0, 500, 100, 100);
        s_AxisPipeline->Bind();

        Matrix4x4 vectorModel = Matrix4x4::identity;
#if LEFT_HANDED
        vectorModel.columns[2].z *= -1;
#endif
        Matrix4x4 vectorView = Matrix4x4::LookAt(cameraPosition.Normalized(), Vector3D::zero, Vector3D::up);
        Matrix4x4 vectorProj = Matrix4x4::Ortho(-1.25f, 1.25f, -1.25f, 1.25f, 0.03f, 2.0f);

        s_VectorGizmoShader->SetMat4("u_ModelViewProj", vectorProj * vectorView * vectorModel);
        s_Xaxis->Render();
        s_Yaxis->Render();
        s_Zaxis->Render();
        RenderCommand::UseShader(s_PointGizmoShader);
        s_PointGizmoShader->SetMat4("u_ModelViewProj", vectorProj * vectorView * vectorModel);
        s_Origin->Render();

        RenderCommand::SetViewport(0, 0, 800, 600);
    }
}
