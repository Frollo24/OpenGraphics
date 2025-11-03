#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Math/Color.h"
#include "OpenEngine/Render/Common.h"
#include "OpenEngine/Render/Texture.h"

namespace OpenGraphics
{
    enum class ClearFlags
    {
        None = 0,
        Color = OG_BIT(0),
        Depth = OG_BIT(1),
        Stencil = OG_BIT(2),
        DepthStencil = Depth | Stencil,
        All = Color | Depth | Stencil
    };

    OG_DEFINE_ENUM_FLAG_OPERATORS(ClearFlags);

    struct ClearValues
    {
        ClearFlags ClearFlags = ClearFlags::None;
        Color Color = Color::white;
        float Depth = 1.0f;
        uint32_t Stencil = 0;
    };

    enum class AttachmentType
    {
        None = 0,
        Color = 1,
        Depth = 2,
        Stencil = 3,
        DepthStencil = 4,
    };

    struct FramebufferDescription
    {
        std::array<AttachmentType, MAX_TOTAL_ATTACHMENTS> Attachments = { AttachmentType::None };
        std::array<Ref<Texture>, MAX_TOTAL_ATTACHMENTS> RenderTargets = { nullptr };
        std::array<ClearValues, MAX_TOTAL_ATTACHMENTS> ClearValues = { {} };
        uint32_t Width = 0;
        uint32_t Height = 0;
    };

    class OPEN_API Framebuffer
    {
    public:
        Framebuffer(const FramebufferDescription& desc);
        Framebuffer(FramebufferDescription&& desc);
        ~Framebuffer();

        [[nodiscard]] inline const FramebufferDescription& GetDescription() const { return m_FramebufferDesc; }
        [[nodiscard]] inline uint32_t GetRendererID() const { return m_RendererID; }

        void Resize(uint32_t width, uint32_t height);

    private:
        void CreateAttachments();

    private:
        FramebufferDescription m_FramebufferDesc;
        uint32_t m_RendererID = 0;
        uint32_t m_RenderTargetCount = 0;
    };
}
