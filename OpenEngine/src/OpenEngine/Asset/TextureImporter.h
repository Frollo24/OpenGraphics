#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Render/Texture.h"  // NOTE: enum ImageFormat, maybe we can declare it elsewhere

namespace OpenGraphics
{
    struct ImageData
    {
        void* Data = nullptr;
        uint32_t Size = 0;
        int32_t Width = 0;
        int32_t Height = 0;
        int32_t Channels = 0;
        ImageFormat Format = ImageFormat::None;
    };

    class OPEN_API TextureImporter
    {
    public:
        static ImageData LoadImage(const std::string& path, bool flipOnLoad = true);
        static std::array<ImageData, 6> LoadCubemapFromSeparateFaces(const std::string& folder, const std::array<std::string, 6>& facesNames);
        static void FreeImageData(const void* imageData);
    };
} // OpenGraphics
