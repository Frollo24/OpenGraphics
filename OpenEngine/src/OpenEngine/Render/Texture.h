#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Math/Color.h"

namespace OpenGraphics
{
    struct ImageExtent
    {
        uint32_t Width;
        uint32_t Height;
        uint32_t Depth;
    };

    enum class ImageType : uint8_t
    {
        Image1D,
        Image2D,
        Image3D,
        Cubemap
    };

    enum class ImageFormat : uint8_t
    {
        None = 0,
        RGB8,
        RGBA8,
        RGB9E5,
        RG16F,
        RGB16F,
        RGBA16F,
        DEPTH32
    };

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

        [[nodiscard]] inline uint32_t GetRendererID() const { return m_RendererID; }
        [[nodiscard]] inline const TextureDescription& GetDescription() const { return m_TextureDesc; }

        void SetData(const void* data);
        void BindTextureUnit(uint32_t textureUnit);

    private:
        TextureDescription m_TextureDesc{};
        uint32_t m_RendererID = 0;
    };
}
