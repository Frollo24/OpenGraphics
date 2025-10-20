#include "ogpch.h"
#include "RenderWorkflow.h"
#include "RenderCommand.h"
#include "RenderingData.h"

// HACK: this shouldn't be directly included here
#include "OpenEngine/Elements/VectorGizmo.h"
#include "OpenEngine/Elements/PointGizmo.h"

namespace OpenGraphics
{
    void RenderWorkflow::Render(const std::vector<const RenderCamera*>& cameras) const
    {
        for (const auto& camera : cameras)
        {
            m_SceneRenderer->BeginCamera(*camera);
            DrawGameObjects(*camera);
            DrawSkybox(*camera);
            DrawGizmos(*camera);
            m_SceneRenderer->EndCamera();
        }
    }

    void RenderWorkflow::DrawGameObjects(const RenderCamera& camera) const
    {
        // NOTE: This should be called per material type
        m_SceneRenderer->SetPipeline(RenderingData::GetGameObjectsPipeline());

        for (const auto scene = m_SceneRenderer->GetScene();
            const auto& gameObject : scene->GetGameObjects())
            gameObject->OnRender();
    }

    void RenderWorkflow::DrawSkybox(const RenderCamera& camera) const
    {
        const auto& skyboxPipeline = RenderingData::GetSkyboxPipeline();
        skyboxPipeline.Bind();
        RenderingData::GetSkyboxCubemapTexture()->BindTextureUnit(0);

        Matrix4x4 skyboxView = camera.GetView();
        const Matrix4x4& skyboxProj = camera.GetProjection();

        if (m_SceneRenderer->IsLeftHanded())
        {
            skyboxView.Scale(Vector3D(1, 1, -1));

            // HACK: should be configured along with the scene handedness
            RenderCommand::SetPolygonState({});
        }

        skyboxView.columns[3] = Vector4D::zero;
        skyboxView.columns[3].w = 1.0f;
        const Matrix4x4 skyboxViewProj = skyboxProj * skyboxView;

        skyboxPipeline.GetShader()->SetMat4("u_ViewProj", skyboxViewProj);
        RenderCommand::BindVertexArray(&RenderingData::GetSkyboxVertexArray());
        RenderCommand::SetVertexBuffer(&RenderingData::GetSkyboxVertexBuffer(), GetSkyboxVertexBinding());
        RenderCommand::SetIndexBuffer(&RenderingData::GetSkyboxIndexBuffer());
        RenderCommand::DrawIndexed(36);
    }

    void RenderWorkflow::DrawGizmos(const RenderCamera& camera) const
    {
        static const VectorGizmo xAxis = VectorGizmo(Vector3D::zero, Vector3D::right, Color(1.0f, 0.0f, 0.0f, 1.0f));
        static const VectorGizmo yAxis = VectorGizmo(Vector3D::zero, Vector3D::up, Color(0.0f, 1.0f, 0.0f, 1.0f));
        static const VectorGizmo zAxis = VectorGizmo(Vector3D::zero, Vector3D::forward, Color(0.0f, 0.0f, 1.0f, 1.0f));
        static const PointGizmo origin = PointGizmo(Vector3D::zero, Color(0.6f, 0.6f, 0.6f, 1.0f));

        const Vector3D cameraPosition = camera.GetPosition();

        RenderCommand::SetViewport(0, 500, 100, 100);

        const auto& gizmosPipeline = RenderingData::GetGizmosPipeline();
        gizmosPipeline.Bind();

        Matrix4x4 vectorModel = Matrix4x4::identity;
        if (m_SceneRenderer->IsLeftHanded())
            vectorModel.columns[2].z *= -1;

        const Matrix4x4 vectorView = Matrix4x4::LookAt(cameraPosition.Normalized(), Vector3D::zero, Vector3D::up);
        const Matrix4x4 vectorProj = Matrix4x4::Ortho(-1.25f, 1.25f, -1.25f, 1.25f, 0.03f, 2.0f);

        gizmosPipeline.GetShader()->SetMat4("u_ModelViewProj", vectorProj * vectorView * vectorModel);
        xAxis.Render();
        yAxis.Render();
        zAxis.Render();
        RenderCommand::UseShader(RenderingData::GetPointGizmoShader());
        RenderingData::GetPointGizmoShader()->SetMat4("u_ModelViewProj", vectorProj * vectorView * vectorModel);
        origin.Render();

        RenderCommand::SetViewport(0, 0, 800, 600);
    }
}
