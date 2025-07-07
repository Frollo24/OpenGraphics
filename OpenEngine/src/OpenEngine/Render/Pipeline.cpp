#include "ogpch.h"
#include "Pipeline.h"

#include "RenderCommand.h"

namespace OpenGraphics
{
    Pipeline::Pipeline(const PipelineState& state, const Ref<Shader>& shader)
        : m_State(state), m_Shader(shader)
    {
    }

    void Pipeline::Bind() const
    {
        RenderCommand::SetPolygonState(m_State.PolygonState);
        RenderCommand::SetDepthState(m_State.DepthState);
        RenderCommand::SetBlendState(m_State.BlendState);
        RenderCommand::UseShader(m_Shader);
    }
}
