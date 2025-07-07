#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Render/Shader.h"
#include "OpenEngine/Render/PipelineState.h"

namespace OpenGraphics
{
    struct PipelineState
    {
        PipelineDepthState DepthState{};
        PipelineBlendState BlendState{};
        PipelinePolygonState PolygonState{};
    };

    class OPEN_API Pipeline
    {
    public:
        Pipeline(const PipelineState& state, const Ref<Shader>& shader);
        ~Pipeline() = default;

        void Bind() const;

        [[nodiscard]] inline const PipelineState& GetPipelineState() const { return m_State; }
        [[nodiscard]] inline const Ref<Shader>& GetShader() const { return m_Shader; }

    private:
        PipelineState m_State{};
        Ref<Shader> m_Shader = nullptr;
    };
}

