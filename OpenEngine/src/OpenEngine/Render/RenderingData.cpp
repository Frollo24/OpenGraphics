#include "ogpch.h"
#include "RenderingData.h"

#include "RenderStructs.h"

namespace OpenGraphics
{
    struct RenderingDataImpl
    {
        Buffer* SkyboxVertexBuffer = nullptr;
        Buffer* SkyboxIndexBuffer = nullptr;
        VertexArray* SkyboxVertexArray = nullptr;

        Ref<Texture> BlackTexture = nullptr;
        Ref<Texture> WhiteTexture = nullptr;
        Ref<Texture> CheckerboardTexture = nullptr;
        Ref<Texture> ErrorCheckerboardTexture = nullptr;
    };

    static RenderingDataImpl* s_RenderingData = nullptr;

    void RenderingData::Init()
    {
        s_RenderingData = new RenderingDataImpl;

        BufferDescription skyboxVertexDescription = {};
        skyboxVertexDescription.Type = BufferType::Vertex;
        skyboxVertexDescription.Size = g_SkyboxVertices.size() * sizeof(Vector3D);
        skyboxVertexDescription.Data = g_SkyboxVertices.data();
        s_RenderingData->SkyboxVertexBuffer = new Buffer(skyboxVertexDescription);

        BufferDescription skyboxIndexDescription = {};
        skyboxIndexDescription.Type = BufferType::Index;
        skyboxIndexDescription.Size = g_SkyboxIndices.size() * sizeof(uint32_t);
        skyboxIndexDescription.Data = g_SkyboxIndices.data();
        s_RenderingData->SkyboxIndexBuffer = new Buffer(skyboxIndexDescription);

        s_RenderingData->SkyboxVertexArray = new VertexArray({GetSkyboxVertexBinding()});

        constexpr uint32_t black = 0x00000000;
        constexpr uint32_t white = 0xffffffff;
        constexpr uint32_t gray = 0x88888888;
        constexpr uint32_t magenta = 0xffff00ff;

        TextureDescription onePixelTextureDesc = {};
        onePixelTextureDesc.ImageExtent = { 1, 1, 1 };
        onePixelTextureDesc.ImageFormat = ImageFormat::RGBA8;
        onePixelTextureDesc.GenerateMipmaps = false;
        onePixelTextureDesc.FilterMode = TextureFilterMode::Nearest;

        s_RenderingData->BlackTexture = CreateRef<Texture>(onePixelTextureDesc);
        s_RenderingData->WhiteTexture = CreateRef<Texture>(onePixelTextureDesc);
        s_RenderingData->BlackTexture->SetData(&black);
        s_RenderingData->WhiteTexture->SetData(&white);

        TextureDescription checkerTextureDesc = {};
        checkerTextureDesc.ImageExtent = { 16, 16, 1 };
        checkerTextureDesc.ImageFormat = ImageFormat::RGBA8;
        checkerTextureDesc.GenerateMipmaps = false;
        checkerTextureDesc.FilterMode = TextureFilterMode::Nearest;
        s_RenderingData->CheckerboardTexture = CreateRef<Texture>(checkerTextureDesc);
        s_RenderingData->ErrorCheckerboardTexture = CreateRef<Texture>(checkerTextureDesc);

        std::array<uint32_t, 16 * 16> checkerboardPixels = { 0 }, errorCheckerboardPixels = { 0 };
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                checkerboardPixels[x * 16 + y] = ((x % 2) ^ (y % 2)) ? magenta : black;
                errorCheckerboardPixels[x * 16 + y] = ((x % 2) ^ (y % 2)) ? white : gray;
            }
        }
        s_RenderingData->CheckerboardTexture->SetData(checkerboardPixels.data());
        s_RenderingData->ErrorCheckerboardTexture->SetData(errorCheckerboardPixels.data());
    }

    void RenderingData::Shutdown()
    {
        delete s_RenderingData->SkyboxVertexArray;
        delete s_RenderingData->SkyboxVertexBuffer;
        delete s_RenderingData->SkyboxIndexBuffer;

        s_RenderingData->WhiteTexture = nullptr;
        s_RenderingData->BlackTexture = nullptr;
        s_RenderingData->CheckerboardTexture = nullptr;
        s_RenderingData->ErrorCheckerboardTexture = nullptr;

        delete s_RenderingData;
        s_RenderingData = nullptr;
    }

    const Buffer& RenderingData::GetSkyboxVertexBuffer()
    {
        return *s_RenderingData->SkyboxVertexBuffer;
    }

    const Buffer& RenderingData::GetSkyboxIndexBuffer()
    {
        return *s_RenderingData->SkyboxIndexBuffer;
    }

    const VertexArray& RenderingData::GetSkyboxVertexArray()
    {
        return *s_RenderingData->SkyboxVertexArray;
    }

    const Texture& RenderingData::GetBlackTexture()
    {
        return *s_RenderingData->BlackTexture;
    }

    const Texture& RenderingData::GetWhiteTexture()
    {
        return *s_RenderingData->WhiteTexture;
    }

}