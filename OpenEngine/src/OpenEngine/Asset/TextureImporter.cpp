#include "ogpch.h"
#include "TextureImporter.h"

#include <stb_image.h>

namespace OpenGraphics
{
    ImageData TextureImporter::LoadImage(const std::string& path, const bool flipOnLoad)
    {
        ImageData imageData{};
        int width, height, channels;
        stbi_set_flip_vertically_on_load(flipOnLoad);
        if (stbi_is_hdr(path.c_str()))
        {
            imageData.Data = stbi_loadf(path.c_str(), &width, &height, &channels,0);
            imageData.Size = width * height * channels * sizeof(float);
            imageData.Format = channels == 4 ? ImageFormat::RGBA32F : ImageFormat::RGB32F;
        }
        else
        {
            imageData.Data = stbi_load(path.c_str(), &width, &height, &channels,0);
            imageData.Size = width * height * channels;
            imageData.Format = channels == 4 ? ImageFormat::RGBA8 : ImageFormat::RGB8;
        }

        if (!imageData.Data)
            return  {};

        imageData.Width = width;
        imageData.Height = height;
        imageData.Channels = channels;
        return imageData;
    }

    std::array<ImageData, 6> TextureImporter::LoadCubemapFromSeparateFaces(const std::string& folder,
        const std::array<std::string, 6>& facesNames)
    {
        std::array<ImageData, 6> cubemapData{};
        for (size_t i = 0; i < facesNames.size(); i++)
        {
            std::string path = std::string(folder) + std::string(facesNames[i]);
            const ImageData faceData = LoadImage(path, false);
            cubemapData[i] = faceData;
        }
        return cubemapData;
    }

    void TextureImporter::FreeImageData(const void* imageData)
    {
        stbi_image_free(const_cast<void*>(imageData));
    }
} // OpenGraphics