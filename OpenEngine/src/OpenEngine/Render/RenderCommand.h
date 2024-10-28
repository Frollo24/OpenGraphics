#pragma once

#include "OpenEngine/Core/Build.h"

#include "OpenEngine/Render/Buffer.h"
#include "OpenEngine/Render/VertexArray.h"

namespace OpenGraphics
{
    class OPEN_API RenderCommand
    {
    public:
        static void BeginFrame();
        static void EndFrame();

        static void BindVertexBuffer(const VertexArray* vertexArray, const Buffer* vertexBuffer, const VertexAttribBinding& binding);

        static void Draw(uint32_t vertexCount);
    };
}
