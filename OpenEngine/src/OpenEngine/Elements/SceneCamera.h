#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Render/RenderCamera.h"
#include "OpenEngine/Scene/GameObject.h"

namespace OpenGraphics
{
    class OPEN_API SceneCamera final : public RenderCamera
    {
    public:
        SceneCamera() = default;
        SceneCamera(const Matrix4x4& projection, GameObject* gameObject)
            : RenderCamera(projection)
            , m_GameObject(gameObject)
        {}
        SceneCamera(const float fov, const float aspect, const float nearPlane, const float farPlane, GameObject* gameObject)
            : RenderCamera(Matrix4x4::Perspective(fov, aspect, nearPlane, farPlane))
            , m_GameObject(gameObject)
        {}
        virtual ~SceneCamera() = default;

        void Update();

    private:
        GameObject* m_GameObject = nullptr;
    };
}
