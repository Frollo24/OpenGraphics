#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/Component.h"
#include "OpenEngine/Elements/Model.h"

namespace OpenGraphics
{
    class OPEN_API RenderComponent : public Component
    {
    public:
        RenderComponent() = default;
        ~RenderComponent();

        [[nodiscard]] inline Model* GetModel() const { return m_Model; }
        inline void SetModel(Model* model) { m_Model = model; }

        void OnRender() override;

    // HACK: A general render component should not have a model
    private:
        Model* m_Model = nullptr;
    };
}