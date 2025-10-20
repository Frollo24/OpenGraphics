#include "ogpch.h"
#include "SceneRenderer.h"

#include "OpenEngine/Scene/CameraComponent.h"
#include "OpenEngine/Render/RenderCommand.h"
#include "OpenEngine/Render/RenderingData.h"

namespace OpenGraphics
{
    SceneRenderer::SceneRenderer(const Scene* scene, const bool leftHanded)
        : m_Scene(const_cast<Scene*>(scene)), m_IsLeftHanded(leftHanded)
    {

    }

    std::vector<const RenderCamera*> SceneRenderer::GetCameras() const
    {
        std::vector<const RenderCamera*> cameras;

        const bool hasEditorCamera = m_EditorCamera != nullptr;
        if (hasEditorCamera)
            cameras.reserve(m_Scene->GetGameObjects().size() + 1);
        else
            cameras.reserve(m_Scene->GetGameObjects().size());

        if (hasEditorCamera)
            cameras.push_back(m_EditorCamera);

        for (const auto& gameObject : m_Scene->GetGameObjects())
        {
            if (const auto& camera = gameObject->GetComponent<CameraComponent>())
            {
                cameras.push_back(camera->GetSceneCamera());
            }
        }

        return cameras;
    }

    void SceneRenderer::BeginRendering()
    {
        // TODO: select whether to render to a render texture or to the default framebuffer
        // TODO: select whether to start direct rendering of recorded rendering
    }

    void SceneRenderer::SetPipeline(const Pipeline& pipeline)
    {
        m_SelectedShader = pipeline.GetShader();
        pipeline.Bind();
        if (!m_SelectedCamera)
            return;

        Vector3D cameraPosition = m_SelectedCamera->GetPosition();
        if (m_IsLeftHanded)
            cameraPosition.z *= -1;

        m_SelectedShader->SetFloat3("u_CameraPosition", cameraPosition);
    }

    void SceneRenderer::BeginCamera(const RenderCamera& renderCamera)
    {
        m_SelectedCamera = const_cast<RenderCamera*>(&renderCamera);
    }

    void SceneRenderer::EndCamera()
    {
        m_SelectedCamera = nullptr;
    }

    void SceneRenderer::EndRendering()
    {
        // TODO: check not to call this method if BeginRendering hasn't been called before
        // TODO: submit commands if recorded rendering was started
    }

    void SceneRenderer::DrawModel(const Model& model, const Transform& transform) const
    {
        std::unordered_multimap<Material, const Mesh&> mesh_drawings;

        for (const auto& mesh: model.GetMeshes())
        {
            mesh_drawings.insert(decltype(mesh_drawings)::value_type(mesh.GetMaterial(), mesh));
        }

        for (const auto& [material, mesh]: mesh_drawings)
        {
            DrawMesh(mesh, transform, material);
        }
    }

    void SceneRenderer::DrawMesh(const Mesh& mesh, const Transform& transform, const Material& material) const
    {
        Matrix4x4 model = transform.GetModelMatrix();
        Matrix4x4 view = m_SelectedCamera->GetView();
        const Matrix4x4 proj = m_SelectedCamera->GetProjection();

        if (m_IsLeftHanded)
        {
            model.Scale(Vector3D(1, 1, -1));
            view.Scale(Vector3D(1, 1, -1));
        }

        const Matrix4x4 normal = Matrix4x4(model).Inverse().Transpose();
        const Matrix4x4 modelViewProj = proj * view * model;

        const Color mainColor = material.MainColor;
        const Color specularColor = material.GetColor("_SpecularColor");
        const Color emissiveColor = material.GetColor("_EmissiveColor");

        const float metallic = material.GetFloat("_Metallic");
        const float roughness = material.GetFloat("_Roughness");

        // NOTE: This should be a property of any material instance
        RenderingData::GetWhiteTexture()->BindTextureUnit(0);
        RenderingData::GetWhiteTexture()->BindTextureUnit(1);
        RenderingData::GetWhiteTexture()->BindTextureUnit(2);

        m_SelectedShader->SetMat4("u_ModelViewProj", modelViewProj);
        m_SelectedShader->SetMat4("u_Model", model);
        m_SelectedShader->SetMat4("u_Normal", normal);
        m_SelectedShader->SetColor("u_MainColor", mainColor);
        m_SelectedShader->SetColor("u_Material.diffuseColor", mainColor);
        m_SelectedShader->SetColor("u_Material.specularColor", specularColor);
        m_SelectedShader->SetColor("u_Material.emissiveColor", emissiveColor);
        m_SelectedShader->SetColor("u_Material.baseColor", mainColor);
        m_SelectedShader->SetFloat("u_Material.metallic", metallic);
        m_SelectedShader->SetFloat("u_Material.roughness", roughness);
        mesh.Render();
    }
}
