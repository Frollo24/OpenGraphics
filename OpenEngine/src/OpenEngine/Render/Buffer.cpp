//
// Created by Adrian on 29/10/2024.
//

#include "Buffer.h"

#include <glad/glad.h>

namespace OpenGraphics
{
    static GLenum BufferUsageToOpenGLBufferUsage(BufferUsage usage)
    {
        switch (usage)
        {
            using enum OpenGraphics::BufferUsage;
            case StreamDraw:  return GL_STREAM_DRAW;
            case StreamRead:  return GL_STREAM_READ;
            case StreamCopy:  return GL_STREAM_COPY;
            case StaticDraw:  return GL_STATIC_DRAW;
            case StaticRead:  return GL_STATIC_READ;
            case StaticCopy:  return GL_STATIC_COPY;
            case DynamicDraw: return GL_DYNAMIC_DRAW;
            case DynamicRead: return GL_DYNAMIC_READ;
            case DynamicCopy: return GL_DYNAMIC_COPY;
            default:
                return GL_NONE;
        }
    }

    Buffer::Buffer(const BufferDescription& desc)
        : m_BufferDesc(desc)
    {
        // TODO: customize desired buffer usage
        glCreateBuffers(1, &m_RendererID);
        glNamedBufferData(m_RendererID, desc.Size, desc.Data, BufferUsageToOpenGLBufferUsage(desc.Usage));
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