#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Math/Color.h"
#include "OpenEngine/Elements/Image.h"

namespace OpenGraphics
{
    enum class TextureFilterMode : uint8_t
    {
        Nearest = 0,
        Linear
    };

    enum class TextureMipmapFilterMode : uint8_t
    {
        NoMipmap = 0,
        NearestMipmap,
        LinearMipmap
    };

    enum class TextureWrapMode : uint8_t
    {
        Repeat = 0,
        ClampEdge,
        ClampBorder,
        Mirror,
        MirrorOnce
    };

    enum class SampleCount : uint8_t
    {
        Count64 = 64,
        Count32 = 32,
        Count16 = 16,
        Count8 = 8,
        Count4 = 4,
        Count2 = 2,
        Count1 = 1,
    };

    struct TextureDescription
    {
        ImageExtent ImageExtent{1, 1, 1};
        ImageType ImageType = ImageType::Image2D;
        ImageFormat ImageFormat = ImageFormat::RGBA8;
        TextureFilterMode FilterMode = TextureFilterMode::Linear;
        TextureMipmapFilterMode MipmapMode = TextureMipmapFilterMode::LinearMipmap;
        TextureWrapMode WrapMode = TextureWrapMode::Repeat;
        Color BorderColor = {0.0f, 0.0f, 0.0f, 1.0f};
        SampleCount SampleCount = SampleCount::Count1;
        bool GenerateMipmaps = true;
    };

    class OPEN_API Texture
    {
    public:
        Texture(const TextureDescription& desc);
        ~Texture();

        [[nodiscard]] inline const TextureDescription& GetDescription() const { return m_TextureDesc; }
        [[nodiscard]] inline uint32_t GetRendererID() const { return m_RendererID; }

        void SetData(const void* data);
        void BindTextureUnit(uint32_t textureUnit) const;

    private:
        TextureDescription m_TextureDesc{};
        uint32_t m_RendererID = 0;
    };
}
