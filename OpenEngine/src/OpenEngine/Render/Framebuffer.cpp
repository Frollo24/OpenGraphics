#include "ogpch.h"
#include "Framebuffer.h"

#include <glad/glad.h>

namespace OpenGraphics
{
    static GLenum AttachmentTypeToOpenGLAttachmentType(const AttachmentType attachment)
    {
        switch (attachment)
        {
            using enum OpenGraphics::AttachmentType;
            case None:         return GL_NONE;
            case Color:        return GL_COLOR_ATTACHMENT0;
            case Depth:        return GL_DEPTH_ATTACHMENT;
            case Stencil:      return GL_STENCIL_ATTACHMENT;
            case DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
            default:
                return GL_NONE;
        }
    }

    Framebuffer::Framebuffer(const FramebufferDescription& desc)
        : m_FramebufferDesc(desc)
    {
        glCreateFramebuffers(1, &m_RendererID);
        CreateAttachments();
    }

    Framebuffer::Framebuffer(FramebufferDescription&& desc)
        : m_FramebufferDesc(std::move(desc))
    {
        glCreateFramebuffers(1, &m_RendererID);
        CreateAttachments();
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
    }

    void Framebuffer::Resize(const uint32_t width, const uint32_t height)
    {
        m_FramebufferDesc.Width = width;
        m_FramebufferDesc.Height = height;

        std::vector<TextureDescription> resizedTextureDescs;
        resizedTextureDescs.reserve(m_RenderTargetCount);

        for (auto& renderTarget : m_FramebufferDesc.RenderTargets)
        {
            if (!renderTarget)
                break;

            TextureDescription newTextureDesc{};
            newTextureDesc.ImageExtent = { width, height, 1};
            newTextureDesc.ImageFormat = renderTarget->GetDescription().ImageFormat;
            newTextureDesc.SampleCount = renderTarget->GetDescription().SampleCount;
            resizedTextureDescs.push_back(newTextureDesc);

            renderTarget = nullptr;
        }

        m_FramebufferDesc.RenderTargets = { nullptr };
        m_RenderTargetCount = 0;
        for (size_t i = 0; i < resizedTextureDescs.size(); i++)
            m_FramebufferDesc.RenderTargets[i] = CreateRef<Texture>(resizedTextureDescs[i]);

        CreateAttachments();
        Logger::Trace("Framebuffer {} resized.", m_RendererID);
    }

    void Framebuffer::CreateAttachments()
    {
        GLsizei drawbufferCount = 0;
        for (size_t i = 0; i < m_FramebufferDesc.RenderTargets.size(); i++)
        {
            BorrowRef<Texture> renderTarget = m_FramebufferDesc.RenderTargets[i];
            const AttachmentType& attachment = m_FramebufferDesc.Attachments[i];
            if (!renderTarget)
                break;

            GLenum openglAttachment = AttachmentTypeToOpenGLAttachmentType(attachment);
            if (openglAttachment == GL_COLOR_ATTACHMENT0)
                openglAttachment += drawbufferCount++;

            glNamedFramebufferTexture(m_RendererID, openglAttachment, renderTarget->GetRendererID(), 0);
            m_RenderTargetCount++;
        }

        if (drawbufferCount > 0)
        {
            std::array<GLenum, MAX_COLOR_ATTACHMENTS> drawbuffers = {
                GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7
            };
            glNamedFramebufferDrawBuffers(m_RendererID, drawbufferCount, drawbuffers.data());
        }
        else
        {
            glNamedFramebufferDrawBuffer(m_RendererID, GL_NONE);
        }

        OG_ASSERT(glCheckNamedFramebufferStatus(m_RendererID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
            "Framebuffer is incomplete!");
    }
}
