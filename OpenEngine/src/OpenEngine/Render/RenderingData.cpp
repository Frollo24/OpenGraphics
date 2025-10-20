#include "ogpch.h"
#include "RenderingData.h"

#include "RenderStructs.h"

#include "OpenEngine/Asset/TextureImporter.h"
#include "OpenEngine/Render/Pipeline.h"

namespace OpenGraphics
{
    struct RenderingDataImpl
    {
        Buffer* SkyboxVertexBuffer = nullptr;
        Buffer* SkyboxIndexBuffer = nullptr;
        VertexArray* SkyboxVertexArray = nullptr;
        Ref<Texture> SkyboxCubemapTexture = nullptr;

        Ref<Texture> BlackTexture = nullptr;
        Ref<Texture> WhiteTexture = nullptr;
        Ref<Texture> CheckerboardTexture = nullptr;
        Ref<Texture> ErrorCheckerboardTexture = nullptr;
    };

    static RenderingDataImpl* s_RenderingData = nullptr;

    // GameObjects
    static Ref<Shader> s_ModelShader = nullptr;
    static Pipeline* s_ModelPipeline = nullptr;

    // Gizmos
    static Ref<Shader> s_VectorGizmoShader = nullptr;
    static Ref<Shader> s_PointGizmoShader = nullptr;
    static Pipeline* s_AxisPipeline = nullptr;

    // Skybox
    static Ref<Shader> s_SkyboxShader = nullptr;
    static Pipeline* s_SkyboxPipeline = nullptr;

    static void LoadPipelines()
    {
#pragma region GameObjects
        ShaderSpecs modelVertexShader{};
        modelVertexShader.Filepath = "assets/shaders/ModelShader.vert";
        modelVertexShader.Filepath = "assets/shaders/BlinnPhong.vert";
        modelVertexShader.Filepath = "assets/shaders/DisneyEpicPBR.vert";
        modelVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs modelFragmentShader{};
        modelFragmentShader.Filepath = "assets/shaders/ModelShader.frag";
        modelFragmentShader.Filepath = "assets/shaders/BlinnPhong.frag";
        modelFragmentShader.Filepath = "assets/shaders/DisneyEpicPBR.frag";
        modelFragmentShader.Type = ShaderType::Fragment;
        s_ModelShader = new Shader({modelVertexShader, modelFragmentShader});

        PipelineState modelPipelineState{};
        s_ModelPipeline = new Pipeline(modelPipelineState, s_ModelShader);
#pragma endregion

#pragma region Skybox
        ShaderSpecs skyboxVertexShader{};
        skyboxVertexShader.Filepath = "assets/shaders/Skybox.vert";
        skyboxVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs skyboxFragmentShader{};
        skyboxFragmentShader.Filepath = "assets/shaders/Skybox.frag";
        skyboxFragmentShader.Type = ShaderType::Fragment;
        s_SkyboxShader = new Shader({skyboxVertexShader, skyboxFragmentShader});

        PipelineState skyboxPipelineState{};
        skyboxPipelineState.PolygonState.FrontFace = FrontFaceMode::Clockwise;
        skyboxPipelineState.DepthState.DepthFunc = DepthComparison::LessOrEqual;
        skyboxPipelineState.DepthState.DepthWrite = false;
        s_SkyboxPipeline = new Pipeline(skyboxPipelineState, s_SkyboxShader);
#pragma endregion

#pragma region Gizmos
        ShaderSpecs axisVertexShader{};
        axisVertexShader.Filepath = "assets/shaders/VectorGizmo.vert";
        axisVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs axisFragmentShader{};
        axisFragmentShader.Filepath = "assets/shaders/VectorGizmo.frag";
        axisFragmentShader.Type = ShaderType::Fragment;
        s_VectorGizmoShader = new Shader({axisVertexShader, axisFragmentShader});

        ShaderSpecs pointVertexShader{};
        pointVertexShader.Filepath = "assets/shaders/PointGizmo.vert";
        pointVertexShader.Type = ShaderType::Vertex;
        ShaderSpecs pointFragmentShader{};
        pointFragmentShader.Filepath = "assets/shaders/PointGizmo.frag";
        pointFragmentShader.Type = ShaderType::Fragment;
        s_PointGizmoShader = new Shader({pointVertexShader, pointFragmentShader});

        // Could be two different pipelines, but with SetPrimitiveTopology command
        // it is no longer necessary
        PipelineState axisPipelineState{};
        s_AxisPipeline = new Pipeline(axisPipelineState, s_VectorGizmoShader);
#pragma endregion
    }

    static void UnloadPipelines()
    {
#pragma region GameObjects
        s_ModelShader = nullptr;
        delete s_ModelPipeline;
#pragma endregion

#pragma region Skybox
        s_SkyboxShader = nullptr;
        delete s_SkyboxPipeline;
#pragma endregion

#pragma region Gizmos
        s_VectorGizmoShader = nullptr;
        s_PointGizmoShader = nullptr;
        delete s_AxisPipeline;
#pragma endregion
    }

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

        // Skybox
        auto cubemapFaces = TextureImporter::LoadCubemapFromSeparateFaces("assets/textures/skyboxFaces/",
            { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" }
        );

        std::array<void*, 6> cubemapData = {nullptr};
        for (int i = 0; i < cubemapFaces.size(); i++)
            cubemapData[i] = cubemapFaces[i].Data;

        TextureDescription cubemapDesc = {};
        cubemapDesc.ImageType = ImageType::Cubemap;
        cubemapDesc.ImageExtent = { cubemapFaces[0].Width, cubemapFaces[0].Height, 1};
        cubemapDesc.ImageFormat = ImageFormat::RGB8;
        cubemapDesc.FilterMode = TextureFilterMode::Nearest;
        cubemapDesc.MipmapMode = TextureMipmapFilterMode::LinearMipmap;
        cubemapDesc.GenerateMipmaps = true;
        s_RenderingData->SkyboxCubemapTexture = CreateRef<Texture>(cubemapDesc);
        s_RenderingData->SkyboxCubemapTexture->SetData(cubemapData.data());

        // Common textures
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

        LoadPipelines();
    }

    void RenderingData::Shutdown()
    {
        UnloadPipelines();

        delete s_RenderingData->SkyboxVertexArray;
        delete s_RenderingData->SkyboxVertexBuffer;
        delete s_RenderingData->SkyboxIndexBuffer;
        s_RenderingData->SkyboxCubemapTexture = nullptr;

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

    const Ref<Texture>& RenderingData::GetSkyboxCubemapTexture()
    {
        return s_RenderingData->SkyboxCubemapTexture;
    }

    const Ref<Texture>& RenderingData::GetBlackTexture()
    {
        return s_RenderingData->BlackTexture;
    }

    const Ref<Texture>& RenderingData::GetWhiteTexture()
    {
        return s_RenderingData->WhiteTexture;
    }

    const Pipeline& RenderingData::GetGameObjectsPipeline()
    {
        return *s_ModelPipeline;
    }

    const Pipeline& RenderingData::GetSkyboxPipeline()
    {
        return *s_SkyboxPipeline;
    }

    const Pipeline& RenderingData::GetGizmosPipeline()
    {
        return *s_AxisPipeline;
    }

    const Ref<Shader>& RenderingData::GetPointGizmoShader()
    {
        return s_PointGizmoShader;
    }
}
