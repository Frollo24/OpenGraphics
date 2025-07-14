#pragma once

namespace OpenGraphics
{
    template<typename T, size_t N>
    T* MemoryBuffer<T, N>::At(size_t index)
    {
        return &reinterpret_cast<T*>(m_MemoryBlock)[index];
    }

    template<typename T, size_t N>
    const T* MemoryBuffer<T, N>::At(size_t index) const
    {
        return &reinterpret_cast<const T*>(m_MemoryBlock)[index];
    }

    template<typename T>
    void Ref<T>::IncreaseRefCount() const
    {
        if (m_Pointer)
            m_RefCount->Increase();
    }

    template<typename T>
    void Ref<T>::DecreaseRefCount() const
    {
        if (!m_RefCount) return;

        m_RefCount->Decrease();
        if (m_RefCount->GetReferenceCount() == 0)
        {
            if (m_OwnsRefCount)
            {
                delete m_RefCount;
                m_RefCount = nullptr;
                delete m_Pointer;
                m_Pointer = nullptr;
            }
            else
            {
                std::destroy_at(m_RefCount);
                std::memset(m_RefCount, 0, sizeof(decltype(m_RefCount)));
                m_RefCount = nullptr;
                std::destroy_at(m_Pointer);
                std::memset(m_Pointer, 0, sizeof(decltype(m_Pointer)));
                m_Pointer = nullptr;
            }
        }
    }

    template<typename T, size_t N>
    template<typename... Args>
    constexpr Ref<T> RefArray<T, N>::CreateRef(size_t index, Args&&... args)
    {
        std::construct_at(m_MemoryBlock.At(index), std::forward<Args>(args)...);
        Ref<T> ref = Ref<T>(m_MemoryBlock.At(index), &m_RefCounts[index]);
        ref.m_OwnsRefCount = false;
        return ref;
    }

    template<typename T, size_t N>
    void RefArray<T, N>::AssignRefCounts()
    {
        for (size_t i = 0; i < N; i++)
        {
            const auto& memoryDir = m_MemoryBlock.At(i);
            m_RefCounts[i] = RefCountPtr<T>(memoryDir);
            // m_RefCounts[i].Increase();
        }
    }

    template<typename T, typename Elem>
    constexpr Ref<T> CreateRef(const std::initializer_list<Elem> initializer_list)
    {
        const auto refCount = new RefCountPtr<T>(initializer_list);
        return Ref<T>(refCount->m_Pointer, refCount);
    }

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        const auto refCount = new RefCountPtr<T>(std::forward<Args>(args)...);
        return Ref<T>(refCount->m_Pointer, refCount);
    }
}