#pragma once

#include "OpenEngine/Core/Build.h"

#include <stdint.h>

namespace OpenGraphics
{
    enum class BufferType
    {
        Vertex, Index
    };

    struct BufferDescription
    {
        BufferType Type;
        uint32_t Size = 0;
        void* Data = nullptr;
    };

    class OPEN_API Buffer
    {
    public:
        Buffer(const BufferDescription& desc);
        ~Buffer();

        inline const BufferDescription& GetDescription() const { return m_BufferDesc; }
        inline uint32_t GetRendererID() const { return m_RendererID; }

        virtual void SetData(uint32_t offset, uint32_t size, const void* data);

    private:
        BufferDescription m_BufferDesc;
        uint32_t m_RendererID;
    };

}
