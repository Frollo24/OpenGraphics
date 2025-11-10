#include "ogpch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace OpenGraphics
{
	namespace Utils
	{
		static GLenum PrimitiveTopologyToGLPrimitiveTopology(const PrimitiveTopology topology)
		{
			switch (topology)
			{
				case PrimitiveTopology::Points:        return GL_POINTS;
				case PrimitiveTopology::Lines:         return GL_LINES;
				case PrimitiveTopology::LineStrip:     return GL_LINE_STRIP;
				case PrimitiveTopology::Triangles:     return GL_TRIANGLES;
				case PrimitiveTopology::TriangleStrip: return GL_TRIANGLE_STRIP;
				case PrimitiveTopology::TriangleFan:   return GL_TRIANGLE_FAN;
				default:
					return GL_NONE;
			}
		}

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
		glClearDepthf(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderCommand::EndFrame()
    {
        // Reset buffer writing to true for any buffer
        glDepthMask(GL_TRUE);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    void RenderCommand::BeginRenderPass(const Framebuffer* framebuffer)
	{
		s_RenderState.Framebuffer = const_cast<Framebuffer*>(framebuffer);
		if (!framebuffer)
			return;

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetRendererID());

		const FramebufferDescription& description = framebuffer->GetDescription();
		const GLuint framebufferID = framebuffer->GetRendererID();
		GLuint drawbuffer = 0;
		int attachmentIndex = 0;
		ClearValues clearValues = {};

		for (const AttachmentType& attachment : description.Attachments)
		{
			if (attachment == AttachmentType::None)
				break;

			clearValues = description.ClearValues[attachmentIndex++];
			switch (attachment)
			{
				case AttachmentType::Color:
					if (static_cast<bool>(clearValues.ClearFlags & ClearFlags::Color))
					{
						std::array<GLfloat, 4> clearColor = {
							clearValues.Color.r, clearValues.Color.g, clearValues.Color.b, clearValues.Color.a
						};
						glClearNamedFramebufferfv(framebufferID, GL_COLOR, drawbuffer++, clearColor.data());
					}
					break;
				case AttachmentType::Depth:
					if (static_cast<bool>(clearValues.ClearFlags & ClearFlags::Depth))
					{
						GLfloat depth = clearValues.Depth;
						glClearNamedFramebufferfv(framebufferID, GL_DEPTH, 0, &depth);
					}
					break;
				case AttachmentType::Stencil:
					if (static_cast<bool>(clearValues.ClearFlags & ClearFlags::Stencil))
					{
						GLint stencil = clearValues.Stencil;
						glClearNamedFramebufferiv(framebufferID, GL_STENCIL, 0, &stencil);
					}
					break;
				case AttachmentType::DepthStencil:
					if (clearValues.ClearFlags == ClearFlags::DepthStencil)
					{
						const GLfloat depth = clearValues.Depth;
						const GLint stencil = clearValues.Stencil;
						glClearNamedFramebufferfi(framebufferID, GL_DEPTH_STENCIL, 0, depth, stencil);
					}
					break;
				default:
					break;
			}
		}
    }

    void RenderCommand::EndRenderPass()
	{
		// At the moment this does nothing
    }

    void RenderCommand::DefaultFrameBuffer()
	{
		s_RenderState.Framebuffer = nullptr;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void RenderCommand::SetViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
	{
		glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
			static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    }

    void RenderCommand::BindVertexArray(const VertexArray* vertexArray)
    {
        s_RenderState.VertexArray = const_cast<VertexArray*>(vertexArray);
		if (!vertexArray)
			return;
        glBindVertexArray(vertexArray->GetRendererID());
    }

    void RenderCommand::SetVertexBuffer(const Buffer* vertexBuffer, const VertexAttribBinding& binding)
    {
        const GLuint vertexArrayID = s_RenderState.VertexArray->GetRendererID();
        const GLuint vertexBufferID = vertexBuffer->GetRendererID();
        glBindVertexArray(vertexArrayID);
        glVertexArrayVertexBuffer(vertexArrayID, binding.GetBinding(),
        	vertexBufferID, 0, static_cast<GLsizei>(binding.GetStride()));
    }

    void RenderCommand::SetIndexBuffer(const Buffer *indexBuffer)
    {
        const GLuint vertexArrayID = s_RenderState.VertexArray->GetRendererID();
        const GLuint indexBufferID = indexBuffer->GetRendererID();
        glBindVertexArray(vertexArrayID);
        glVertexArrayElementBuffer(vertexArrayID, indexBufferID);
    }

    void RenderCommand::SetPrimitiveTopology(const PrimitiveTopology& primitiveTopology)
	{
		s_RenderState.PrimitiveTopology = primitiveTopology;
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
				static_cast<GLboolean>(blendAttachment.ColorWriteMask & ColorWriteMask::ColorWriteMaskR),
				static_cast<GLboolean>(blendAttachment.ColorWriteMask & ColorWriteMask::ColorWriteMaskG),
				static_cast<GLboolean>(blendAttachment.ColorWriteMask & ColorWriteMask::ColorWriteMaskB),
				static_cast<GLboolean>(blendAttachment.ColorWriteMask & ColorWriteMask::ColorWriteMaskA)
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

    void RenderCommand::UseShader(BorrowRef<Shader> shader)
    {
		s_RenderState.Shader = shader;
		if (!shader)
			return;
        glUseProgram(shader->GetRendererID());
    }

    void RenderCommand::BindUniformBuffer(const Buffer* uniformBuffer, const uint32_t index)
	{
		OG_ASSERT(uniformBuffer->GetDescription().Type == BufferType::Uniform,
			"Trying to bind a buffer which is not a Uniform Buffer Object!");
		glBindBufferBase(GL_UNIFORM_BUFFER, index, uniformBuffer->GetRendererID());
    }

    void RenderCommand::Draw(const uint32_t first, const uint32_t vertexCount)
    {
		const GLenum topology = Utils::PrimitiveTopologyToGLPrimitiveTopology(s_RenderState.PrimitiveTopology);
        glDrawArrays(topology, static_cast<GLint>(first), static_cast<GLsizei>(vertexCount));
    }

    void RenderCommand::DrawIndexed(const uint32_t indexCount)
    {
		const GLenum topology = Utils::PrimitiveTopologyToGLPrimitiveTopology(s_RenderState.PrimitiveTopology);
        glDrawElements(topology, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
    }
}
