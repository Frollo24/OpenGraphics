#pragma once

#include "OpenEngine/Core/Base.h"

#include "OpenEngine/Render/Buffer.h"
#include "OpenEngine/Render/VertexArray.h"
#include "OpenEngine/Render/RenderStructs.h"

namespace OpenGraphics
{
    class OPEN_API RenderEntity
    {
    public:
        RenderEntity(const std::vector<Vertex>& vertices, const std::vector<Index>& indices);
        ~RenderEntity();

        void Render() const;

    private:
        VertexArray* m_VertexArray = nullptr;
        VertexAttribBinding m_VertexAttribBinding = {};
        Buffer* m_VertexBuffer = nullptr;
        Buffer* m_IndexBuffer = nullptr;
        uint32_t m_VertexCount = 0;
        uint32_t m_IndexCount = 0;
    };

}


