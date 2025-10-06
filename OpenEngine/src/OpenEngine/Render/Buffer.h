#pragma once

#include "OpenEngine/Core/Base.h"

namespace OpenGraphics
{
    enum class BufferType
    {
        Vertex, Index, Uniform
    };

    // NOTE: It may be preferable to break it down by frequency and type of use
    enum class BufferUsage
    {
        StreamDraw,  StreamRead,  StreamCopy,
        StaticDraw,  StaticRead,  StaticCopy,
        DynamicDraw, DynamicRead, DynamicCopy
    };

    struct BufferDescription
    {
        BufferType Type;
        BufferUsage Usage = BufferUsage::StaticDraw;
        uint32_t Size = 0;
        void* Data = nullptr;
    };

    class OPEN_API Buffer
    {
    public:
        Buffer(const BufferDescription& desc);
        ~Buffer();

        [[nodiscard]] inline const BufferDescription& GetDescription() const { return m_BufferDesc; }
        [[nodiscard]] inline uint32_t GetRendererID() const { return m_RendererID; }

        void SetData(uint32_t offset, uint32_t size, const void* data);

    private:
        BufferDescription m_BufferDesc;
        uint32_t m_RendererID = 0;
    };

}
