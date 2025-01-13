#pragma once

#include "OpenEngine/Core/Base.h"

#include "OpenEngine/Render/Buffer.h"
#include "OpenEngine/Render/VertexArray.h"
#include "OpenEngine/Render/Shader.h"

namespace OpenGraphics
{
    struct RenderState
    {
        VertexArray* VertexArray = nullptr;
    };

    class OPEN_API RenderCommand
    {
    public:
        static void BeginFrame();
        static void EndFrame();

        static void BindVertexArray(const VertexArray* vertexArray);
        static void SetVertexBuffer(const Buffer* vertexBuffer, const VertexAttribBinding& binding);
        static void SetIndexBuffer(const Buffer* indexBuffer);

        // TODO: temp
        // HACK: we should create a proper pipeline instead of binding only the shader
        static void UseShader(const Shader* shader);

        static void Draw(uint32_t vertexCount);
        static void DrawIndexed(uint32_t indexCount);

    private:
        inline static RenderState s_RenderState = {};
    };
}
