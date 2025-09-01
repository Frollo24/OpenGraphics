#include "ogpch.h"
#include "SceneCamera.h"

namespace OpenGraphics
{
    void SceneCamera::Update()
    {
        Matrix4x4 modelMatrix = m_GameObject->GetTransform()->GetModelMatrix();
        m_View = modelMatrix.Inverse();
        const Vector4D position = modelMatrix[3];
        m_Position = Vector3D(position.x, position.y, position.z);
    }
}
