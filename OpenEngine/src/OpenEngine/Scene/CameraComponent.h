#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/Component.h"
#include "OpenEngine/Elements/SceneCamera.h"

namespace OpenGraphics
{
    class OPEN_API CameraComponent final : public Component
    {
    public:
        CameraComponent() = default;
        ~CameraComponent() = default;

        [[nodiscard]] const SceneCamera* GetSceneCamera() const { return m_SceneCamera; }

        void OnUpdate() override;

    private:
        SceneCamera* m_SceneCamera = nullptr;
    };
}
