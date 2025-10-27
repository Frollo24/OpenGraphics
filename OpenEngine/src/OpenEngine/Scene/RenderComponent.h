#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/Component.h"

namespace OpenGraphics
{
    class OPEN_API RenderComponent : public Component
    {
    public:
        RenderComponent() = default;
        virtual ~RenderComponent() = default;
    };
}