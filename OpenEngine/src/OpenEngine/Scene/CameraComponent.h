#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/Component.h"
#include "OpenEngine/Render/RenderCamera.h"

namespace OpenGraphics
{
    class OPEN_API CameraComponent final : public Component
    {
    public:
        CameraComponent() = default;
        ~CameraComponent() = default;

        [[nodiscard]] const RenderCamera* GetRenderCamera() const { return m_RenderCamera; }

    private:
        RenderCamera* m_RenderCamera = nullptr;
    };
}
