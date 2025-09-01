#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Elements/Model.h"
#include "OpenEngine/Elements/EditorCamera.h"
#include "OpenEngine/Render/Pipeline.h"
#include "OpenEngine/Render/RenderCamera.h"
#include "OpenEngine/Scene/Scene.h"

namespace OpenGraphics
{
    class OPEN_API SceneRenderer
    {
    public:
        SceneRenderer(const Scene* scene);

        [[nodiscard]] std::vector<const RenderCamera*> GetCameras() const;

        void BeginRendering();
        void SetPipeline(const Pipeline& pipeline);
        void BeginCamera(const RenderCamera& renderCamera);
        void EndCamera();
        void EndRendering();

        // TODO: implement
        void DrawCube(const Transform& transform);
        void DrawSphere(const Transform& transform);
        void DrawCylinder(const Transform& transform);
        void DrawPlane(const Transform& transform);

        void DrawModel(const Model& model, const Transform& transform) const;
        void DrawMesh(const Mesh& mesh, const Transform& transform, const Material& material) const;

        [[nodiscard]] inline const Scene* GetScene() const { return m_Scene; }

        // TODO: improve interface
        [[nodiscard]] inline const EditorCamera* GetEditorCamera() const { return m_EditorCamera; }
        void SetEditorCamera(EditorCamera* camera) { m_EditorCamera = camera; }

    private:
        Scene* m_Scene = nullptr;
        Ref<Shader> m_SelectedShader = nullptr;
        RenderCamera* m_SelectedCamera = nullptr;
        EditorCamera* m_EditorCamera = nullptr;
    };
}