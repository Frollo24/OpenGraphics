#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/RenderComponent.h"
#include "OpenEngine/Elements/Model.h"

namespace OpenGraphics
{
    class OPEN_API ModelRenderComponent : public RenderComponent
    {
    public:
        ModelRenderComponent() = default;
        ~ModelRenderComponent();

        [[nodiscard]] inline Model* GetModel() const { return m_Model; }
        inline void SetModel(Model* model) { m_Model = model; }

        void OnRender() override;

    private:
        Model* m_Model = nullptr;
    };
}
