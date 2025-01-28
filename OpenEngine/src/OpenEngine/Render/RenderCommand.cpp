#include "ogpch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace OpenGraphics
{
	namespace Utils
	{
		static GLenum DepthComparisonToGLDepthFunc(const DepthComparison comparison)
		{
			switch (comparison)
			{
				case DepthComparison::None:           return GL_NEVER;
				case DepthComparison::Less:			  return GL_LESS;
				case DepthComparison::Equal:		  return GL_EQUAL;
				case DepthComparison::LessOrEqual:	  return GL_LEQUAL;
				case DepthComparison::Greater:		  return GL_GREATER;
				case DepthComparison::NotEqual:		  return GL_NOTEQUAL;
				case DepthComparison::GreaterOrEqual: return GL_GEQUAL;
				case DepthComparison::Always:         return GL_ALWAYS;
				default:
					return GL_NONE;
			}
		}

		static GLenum BlendFactorToGLBlendFactor(const BlendFactor factor)
		{
			switch (factor)
			{
				case BlendFactor::Zero:					 return GL_ZERO;
				case BlendFactor::One:					 return GL_ONE;
				case BlendFactor::SrcColor:				 return GL_SRC_COLOR;
				case BlendFactor::OneMinusSrcColor:		 return GL_ONE_MINUS_SRC_COLOR;
				case BlendFactor::DstColor:				 return GL_DST_COLOR;
				case BlendFactor::OneMinusDstColor:		 return GL_ONE_MINUS_DST_COLOR;
				case BlendFactor::SrcAlpha:				 return GL_SRC_ALPHA;
				case BlendFactor::OneMinusSrcAlpha:		 return GL_ONE_MINUS_SRC_ALPHA;
				case BlendFactor::DstAlpha:				 return GL_DST_ALPHA;
				case BlendFactor::OneMinusDstAlpha:		 return GL_ONE_MINUS_DST_ALPHA;
				case BlendFactor::ConstantColor:		 return GL_CONSTANT_COLOR;
				case BlendFactor::OneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;
				case BlendFactor::ConstantAlpha:		 return GL_CONSTANT_ALPHA;
				case BlendFactor::OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
				default:
					return GL_NONE;
			}
		}

		static GLenum BlendEquationToGLBlendEquation(const BlendOperation equation)
		{
			switch (equation)
			{
				case BlendOperation::Add:             return GL_FUNC_ADD;
				case BlendOperation::Subtract:        return GL_FUNC_SUBTRACT;
				case BlendOperation::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
				case BlendOperation::Minimum:         return GL_MIN;
				case BlendOperation::Maximum:         return GL_MAX;
				default:
					return GL_NONE;
			}
		}

		static GLenum PolygonRasterModeToGLPolygonMode(const PolygonRasterMode polygonMode)
		{
			switch (polygonMode)
			{
				case PolygonRasterMode::Fill:  return GL_FILL;
				case PolygonRasterMode::Line:  return GL_LINE;
				case PolygonRasterMode::Point: return GL_POINT;
				default:
					return GL_NONE;
			}
		}

		static GLenum CullingModeToGLCullFace(const CullingMode culling)
		{
			switch (culling)
			{
				case CullingMode::Front:        return GL_FRONT;
				case CullingMode::Back:         return GL_BACK;
				case CullingMode::FrontAndBack: return GL_FRONT_AND_BACK;
				default:
					return GL_NONE;
			}
		}

		static GLenum FrontFaceModeToGLFrontFace(const FrontFaceMode frontFace)
		{
			switch (frontFace)
			{
				case FrontFaceMode::CounterClockwise: return GL_CCW;
				case FrontFaceMode::Clockwise:        return GL_CW;
				default:
					return GL_NONE;
			}
		}
	}

    void RenderCommand::BeginFrame()
    {
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderCommand::EndFrame()
    {
        // Reset buffer writing to true for any buffer
        glDepthMask(GL_TRUE);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    void RenderCommand::BindVertexArray(const VertexArray* vertexArray)
    {
        s_RenderState.VertexArray = const_cast<VertexArray*>(vertexArray);
        glBindVertexArray(vertexArray->GetRendererID());
    }

    void RenderCommand::SetVertexBuffer(const Buffer* vertexBuffer, const VertexAttribBinding& binding)
    {
        GLuint vertexArrayID = s_RenderState.VertexArray->GetRendererID();
        GLuint vertexBufferID = vertexBuffer->GetRendererID();
        glBindVertexArray(vertexArrayID);
        glVertexArrayVertexBuffer(vertexArrayID, binding.GetBinding(), vertexBufferID, 0, binding.GetStride());
    }

    void RenderCommand::SetIndexBuffer(const Buffer *indexBuffer)
    {
        GLuint vertexArrayID = s_RenderState.VertexArray->GetRendererID();
        GLuint indexBufferID = indexBuffer->GetRendererID();
        glBindVertexArray(vertexArrayID);
        glVertexArrayElementBuffer(vertexArrayID, indexBufferID);
    }

	void RenderCommand::SetDepthState(const PipelineDepthState& depthState)
	{
		if (!depthState.DepthTest)
		{
			glDisable(GL_DEPTH_TEST);
			return;
		}
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(Utils::DepthComparisonToGLDepthFunc(depthState.DepthFunc));
		glDepthRange(depthState.DepthRange.MinDepth, depthState.DepthRange.MaxDepth);
		glDepthMask(depthState.DepthWrite ? GL_TRUE : GL_FALSE);
	}

	void RenderCommand::SetBlendState(const PipelineBlendState& blendState)
	{
		for (int idx = 0; idx < blendState.BlendAttachments.size(); idx++)
		{
			const BlendAttachment& blendAttachment = blendState.BlendAttachments[idx];
			if (!blendAttachment.BlendEnable)
			{
				glDisablei(GL_BLEND, idx);
				continue;
			}
			glEnablei(GL_BLEND, idx);

			glBlendFuncSeparatei(idx,
				Utils::BlendFactorToGLBlendFactor(blendAttachment.ColorEquation.SrcFactor),
				Utils::BlendFactorToGLBlendFactor(blendAttachment.ColorEquation.DstFactor),
				Utils::BlendFactorToGLBlendFactor(blendAttachment.AlphaEquation.SrcFactor),
				Utils::BlendFactorToGLBlendFactor(blendAttachment.AlphaEquation.DstFactor)
			);

			glBlendEquationSeparatei(idx,
				Utils::BlendEquationToGLBlendEquation(blendAttachment.ColorEquation.Operation),
				Utils::BlendEquationToGLBlendEquation(blendAttachment.AlphaEquation.Operation)
			);

			glColorMaski(idx,
				GLboolean(blendAttachment.ColorWriteMask & ColorWriteMask::ColorWriteMaskR),
				GLboolean(blendAttachment.ColorWriteMask & ColorWriteMask::ColorWriteMaskG),
				GLboolean(blendAttachment.ColorWriteMask & ColorWriteMask::ColorWriteMaskB),
				GLboolean(blendAttachment.ColorWriteMask & ColorWriteMask::ColorWriteMaskA)
			);
		}

		glBlendColor(blendState.ConstantColor.R, blendState.ConstantColor.G, blendState.ConstantColor.B, blendState.ConstantColor.A);
	}

	void RenderCommand::SetPolygonState(const PipelinePolygonState& polygonState)
	{
		if (!polygonState.CullEnable)
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);
		glFrontFace(Utils::FrontFaceModeToGLFrontFace(polygonState.FrontFace));
		glCullFace(Utils::CullingModeToGLCullFace(polygonState.CullMode));
		glPolygonMode(GL_FRONT_AND_BACK, Utils::PolygonRasterModeToGLPolygonMode(polygonState.PolygonMode));
	}

    void RenderCommand::UseShader(const Shader* shader)
    {
        glUseProgram(shader->GetRendererID());
    }

    void RenderCommand::Draw(uint32_t vertexCount)
    {
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    void RenderCommand::DrawIndexed(uint32_t indexCount)
    {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }
}
