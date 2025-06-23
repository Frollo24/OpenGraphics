#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Render/Texture.h"
#include "OpenEngine/Render/Buffer.h"
#include "OpenEngine/Render/VertexArray.h"

namespace OpenGraphics
{
    class OPEN_API RenderingData
    {
    public:
        static void Init();
        static void Shutdown();

        static const Buffer& GetSkyboxVertexBuffer();
        static const Buffer& GetSkyboxIndexBuffer();

        static const VertexArray& GetSkyboxVertexArray();

        static const Texture& GetBlackTexture();
        static const Texture& GetWhiteTexture();
    };
}