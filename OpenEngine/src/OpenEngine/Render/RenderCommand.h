#pragma once

#include "OpenEngine/Core/Base.h"

#include "OpenEngine/Render/Buffer.h"
#include "OpenEngine/Render/VertexArray.h"
#include "OpenEngine/Render/Pipeline.h"

namespace OpenGraphics
{
    struct RenderState
    {
        VertexArray* VertexArray = nullptr;
        Shader* Shader = nullptr;
    };

    class OPEN_API RenderCommand
    {
    public:
        static void BeginFrame();
        static void EndFrame();

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        static void BindVertexArray(const VertexArray* vertexArray);
        static void SetVertexBuffer(const Buffer* vertexBuffer, const VertexAttribBinding& binding);
        static void SetIndexBuffer(const Buffer* indexBuffer);

        static void SetDepthState(const PipelineDepthState& depthState);
        static void SetBlendState(const PipelineBlendState& blendState);
        static void SetPolygonState(const PipelinePolygonState& polygonState);
        static void UseShader(const Shader* shader);

        static void Draw(uint32_t vertexCount);
        static void DrawIndexed(uint32_t indexCount);

        static void DrawLines(uint32_t vertexCount);
        static void DrawPoints(uint32_t first, uint32_t vertexCount);

    private:
        inline static RenderState s_RenderState = {};
    };
}
