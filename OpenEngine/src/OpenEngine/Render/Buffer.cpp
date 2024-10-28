//
// Created by Adrian on 29/10/2024.
//

#include "Buffer.h"

#include <glad/glad.h>

namespace OpenGraphics
{
    Buffer::Buffer(const BufferDescription& desc)
        : m_BufferDesc(desc)
    {
        // TODO: customize desired buffer usage
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, desc.Size, desc.Data, GL_STATIC_DRAW);
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void Buffer::SetData(uint32_t offset, uint32_t size, const void* data)
    {
        glNamedBufferSubData(m_RendererID, offset, size, data);
    }
} // OpenGraphics