#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Elements/Image.h"

namespace OpenGraphics
{
    struct ImageData
    {
        void* Data = nullptr;
        uint32_t Size = 0;
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t Channels = 0;
        ImageFormat Format = ImageFormat::None;
    };

    class OPEN_API TextureImporter
    {
    public:
        static ImageData LoadImage(const std::string& path, bool flipOnLoad = true);
        static std::array<ImageData, 6> LoadCubemapFromSeparateFaces(const std::string& folder, const std::array<std::string, 6>& facesNames);
        static void FreeImageData(const void* imageData);
    };
}
