#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Render/Texture.h"
#include "OpenEngine/Render/Buffer.h"
#include "OpenEngine/Render/VertexArray.h"
#include "OpenEngine/Render/Pipeline.h"

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
        static const Ref<Texture>& GetSkyboxCubemapTexture();

        static const Ref<Texture>& GetBlackTexture();
        static const Ref<Texture>& GetWhiteTexture();

        // NOTE: Some of this pipelines are dependent on the assigned gameobject
        static const Pipeline& GetGameObjectsPipeline();
        static const Pipeline& GetSkyboxPipeline();
        static const Pipeline& GetGizmosPipeline();

        // HACK: Should be managed by a somewhat Shader-Manager
        static const Ref<Shader>& GetPointGizmoShader();
    };
}