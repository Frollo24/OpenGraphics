#pragma once

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
        RG32F,
        RGB32F,
        RGBA32F,
        DEPTH32
    };
}
