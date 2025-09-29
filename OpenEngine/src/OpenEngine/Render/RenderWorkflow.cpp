#include "ogpch.h"
#include "RenderWorkflow.h"
#include "RenderCommand.h"

// HACK: this shouldn't be directly included here
#include "RenderStructs.h"
#include "RenderingData.h"
#include "OpenEngine/Asset/TextureImporter.h"
#include "OpenEngine/Elements/VectorGizmo.h"
#include "OpenEngine/Elements/PointGizmo.h"
#include "OpenEngine/Scene/LightComponent.h"
// #include "OpenEngine/Scene/Scene.h"
// #include "OpenEngine/Scene/RenderComponent.h"

// NOTE: could be interesting to leave this as a parameter for the user
#define LEFT_HANDED 0

namespace OpenGraphics
{
    // GameObjects
    static Ref<Shader> s_ModelShader = nullptr;
    static Pipeline* s_ModelPipeline = nullptr;

    // Gizmos
    static VectorGizmo* s_Xaxis = nullptr;
    static VectorGizmo* s_Yaxis = nullptr;
    static VectorGizmo* s_Zaxis = nullptr;
    static PointGizmo* s_Origin = nullptr;
    static Ref<Shader> s_VectorGizmoShader = nullptr;
    static Ref<Shader> s_PointGizmoShader = nullptr;
    static Pipeline* s_AxisPipeline = nullptr;

    // Skybox
    static Ref<Shader> s_SkyboxShader = nullptr;
    static Pipeline* s_SkyboxPipeline = nullptr;
    static Ref<Texture> s_SkyboxCubemap = nullptr;

    void RenderWorkflow::Render(const std::vector<const RenderCamera*>& cameras) const
    {
        for (const auto& camera : cameras)
        {
            DrawGameObjects(*camera);
            DrawSkybox(*camera);
            DrawGizmos(*camera);
        }
    }

    void RenderWorkflow::InitResources()
    {
#pragma region GameObjects
        ShaderSpecs modelVertexShader{};
        modelVertexShader.Filepath = "assets/shaders/ModelShader.vert";
        modelVertexShader.Filepath = "assets/shaders/BlinnPhong.vert";
        modelVertexShader.Filepath = "assets/shaders/DisneyEpicPBR.vert";
        modelVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs modelFragmentShader{};
        modelFragmentShader.Filepath = "assets/shaders/ModelShader.frag";
        modelFragmentShader.Filepath = "assets/shaders/BlinnPhong.frag";
        modelFragmentShader.Filepath = "assets/shaders/DisneyEpicPBR.frag";
        modelFragmentShader.Type = ShaderType::Fragment;
        s_ModelShader = new Shader({modelVertexShader, modelFragmentShader});

        PipelineState modelPipelineState{};
        s_ModelPipeline = new Pipeline(modelPipelineState, s_ModelShader);
#pragma endregion

#pragma region Skybox
        // Skybox
        auto cubemapFaces = TextureImporter::LoadCubemapFromSeparateFaces("assets/textures/skyboxFaces/",
            { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" }
        );

        std::array<void*, 6> cubemapData = {nullptr};
        for (int i = 0; i < cubemapFaces.size(); i++)
            cubemapData[i] = cubemapFaces[i].Data;

        TextureDescription cubemapDesc = {};
        cubemapDesc.ImageType = ImageType::Cubemap;
        cubemapDesc.ImageExtent = { cubemapFaces[0].Width, cubemapFaces[0].Height, 1};
        cubemapDesc.ImageFormat = ImageFormat::RGB8;
        cubemapDesc.FilterMode = TextureFilterMode::Nearest;
        cubemapDesc.MipmapMode = TextureMipmapFilterMode::LinearMipmap;
        cubemapDesc.GenerateMipmaps = true;
        s_SkyboxCubemap = CreateRef<Texture>(cubemapDesc);
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

        // Could be two different pipelines, but with SetPrimitiveTopology command
        // it is no longer necessary
        PipelineState axisPipelineState{};
        s_AxisPipeline = new Pipeline(axisPipelineState, s_VectorGizmoShader);
#pragma endregion
    }

    void RenderWorkflow::DeleteResources()
    {
#pragma region GameObjects
        s_ModelShader = nullptr;
        delete s_ModelPipeline;
#pragma endregion

#pragma region Skybox
        s_SkyboxCubemap = nullptr;
        s_SkyboxShader = nullptr;
        delete s_SkyboxPipeline;
#pragma endregion

#pragma region Gizmos
        delete s_Xaxis;
        delete s_Yaxis;
        delete s_Zaxis;
        delete s_Origin;

        delete s_AxisPipeline;
        s_VectorGizmoShader = nullptr;
        s_PointGizmoShader = nullptr;
#pragma endregion
    }

    void RenderWorkflow::DrawGameObjects(const RenderCamera& camera) const
    {
        // HACK: Light data should be set in a Uniform Buffer
        const auto lightGameObject = m_SceneRenderer->GetScene()->GetGameObjects().at(2);
        const auto lightData = lightGameObject->GetComponent<LightComponent>()->GetLightData();
        const auto shader = s_ModelPipeline->GetShader();
        if (lightData.Type == LightType::Directional)
        {
            shader->SetFloat3("u_DirectionalLight.color",
                Vector3D(lightData.LightColor.r, lightData.LightColor.g, lightData.LightColor.b));
            shader->SetFloat("u_DirectionalLight.intensity", lightData.Intensity);
            shader->SetFloat3("u_DirectionalLight.direction", Vector3D(-1.0, -1.0, -1.0).Normalized());
            shader->SetFloat3("u_PointLight.color", Vector3D::zero);
            shader->SetFloat("u_PointLight.intensity", 0.0f);
            shader->SetFloat3("u_PointLight.position", Vector3D::zero);
            shader->SetFloat("u_PointLight.radius", 0.0f);
        }
        else if (lightData.Type == LightType::Point)
        {
            shader->SetFloat3("u_DirectionalLight.color", Vector3D::zero);
            shader->SetFloat("u_DirectionalLight.intensity", 0.0f);
            shader->SetFloat3("u_DirectionalLight.direction", Vector3D::zero);
            shader->SetFloat3("u_PointLight.color",
                Vector3D(lightData.LightColor.r, lightData.LightColor.g, lightData.LightColor.b));
            shader->SetFloat("u_PointLight.intensity", lightData.Intensity);
            const Vector4D& lastColumn = lightGameObject->GetTransform()->GetModelMatrix()[3];
            shader->SetFloat3("u_PointLight.position", Vector3D(lastColumn.x, lastColumn.y, lastColumn.z));
            shader->SetFloat("u_PointLight.radius", 20.0f);
        }

        // NOTE: This should be called per material type
        m_SceneRenderer->SetPipeline(*s_ModelPipeline);

        // NOTE: This should be called per material instance
        RenderingData::GetWhiteTexture()->BindTextureUnit(0);
        RenderingData::GetWhiteTexture()->BindTextureUnit(1);
        RenderingData::GetWhiteTexture()->BindTextureUnit(2);

        m_SceneRenderer->BeginCamera(camera);
        const auto scene = m_SceneRenderer->GetScene();
        const auto sphereGameObject = scene->GetGameObjects().at(0);
        /*
        Vector3D cameraPosition = camera->GetPosition();
        Matrix4x4 model = sphereGameObject->GetTransform()->GetModelMatrix();
        Matrix4x4 view = camera->GetView();

#if LEFT_HANDED
        model.Scale(Vector3D(1, 1, -1));
        view.Scale(Vector3D(1, 1, -1));
        cameraPosition.z *= -1;
#endif
        Matrix4x4 proj = camera->GetProjection();
        */
        sphereGameObject->OnRender();


        const auto starGameObject = scene->GetGameObjects().at(1);
        /*
        model = starGameObject->GetTransform()->GetModelMatrix();

#if LEFT_HANDED
        model.Scale(Vector3D(1, 1, -1));
#endif
        */
        starGameObject->OnRender();
    }

    void RenderWorkflow::DrawSkybox(const RenderCamera& camera) const
    {
        Matrix4x4 skyboxView = camera.GetView();
        const Matrix4x4& skyboxProj = camera.GetProjection();

#if LEFT_HANDED
        skyboxView.Scale(Vector3D(1, 1, -1));
#endif

        skyboxView.columns[3] = Vector4D::zero;
        skyboxView.columns[3].w = 1.0f;
        const Matrix4x4 skyboxViewProj = skyboxProj * skyboxView;

        s_SkyboxShader->SetMat4("u_ViewProj", skyboxViewProj);
        s_SkyboxPipeline->Bind();
        s_SkyboxCubemap->BindTextureUnit(0);

        RenderCommand::BindVertexArray(&RenderingData::GetSkyboxVertexArray());
        RenderCommand::SetVertexBuffer(&RenderingData::GetSkyboxVertexBuffer(), GetSkyboxVertexBinding());
        RenderCommand::SetIndexBuffer(&RenderingData::GetSkyboxIndexBuffer());
        RenderCommand::DrawIndexed(36);
    }

    void RenderWorkflow::DrawGizmos(const RenderCamera& camera) const
    {
        const Vector3D cameraPosition = camera.GetPosition();

        RenderCommand::SetViewport(0, 500, 100, 100);
        s_AxisPipeline->Bind();

        Matrix4x4 vectorModel = Matrix4x4::identity;
#if LEFT_HANDED
        vectorModel.columns[2].z *= -1;
#endif
        const Matrix4x4 vectorView = Matrix4x4::LookAt(cameraPosition.Normalized(), Vector3D::zero, Vector3D::up);
        const Matrix4x4 vectorProj = Matrix4x4::Ortho(-1.25f, 1.25f, -1.25f, 1.25f, 0.03f, 2.0f);

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
