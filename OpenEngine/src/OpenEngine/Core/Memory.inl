#pragma once

namespace OpenGraphics
{
    template<typename T, size_t N>
    T* MemoryBuffer<T, N>::At(const size_t index)
    {
        return &reinterpret_cast<T*>(m_MemoryBlock)[index];
    }

    template<typename T, size_t N>
    const T* MemoryBuffer<T, N>::At(const size_t index) const
    {
        return &reinterpret_cast<const T*>(m_MemoryBlock)[index];
    }

    template<typename T>
    T* UnboundedMemoryBuffer<T>::At(const size_t index)
    {
        return &reinterpret_cast<T*>(m_MemoryBlock)[index];
    }

    template<typename T>
    const T* UnboundedMemoryBuffer<T>::At(const size_t index) const
    {
        return &reinterpret_cast<const T*>(m_MemoryBlock)[index];
    }

    template<typename T>
    void UnboundedMemoryBuffer<T>::Resize(const size_t newSize)
    {
        alignas(T) auto* newMemoryBlock = new unsigned char[newSize * sizeof(T)];
        std::memcpy(newMemoryBlock, m_MemoryBlock, m_Size * sizeof(T));
        m_Size = newSize;
        delete[] m_MemoryBlock;
        m_MemoryBlock = newMemoryBlock;
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
        }
    }

    template<typename T>
    DynamicRefArray<T>::DynamicRefArray(const size_t count)
        : m_MemoryBlock(count), m_RefCounts(count, new RefCountPtr<T>)
    {
        AssignRefCounts();
    }

    template<typename T>
    template<typename... Args>
    constexpr void DynamicRefArray<T>::EmplaceNew(Args&&... args)
    {
        if (m_MemoryBlock.Size() == m_Count)
            Resize();

        std::construct_at(m_MemoryBlock.At(m_Count), std::forward<Args>(args)...);
        m_Count++;
    }

    template<typename T>
    constexpr Ref<T> DynamicRefArray<T>::CreateRefAt(size_t index)
    {
        Ref<T> ref = Ref<T>(m_MemoryBlock.At(index), m_RefCounts[index]);
        ref.m_OwnsRefCount = false;
        return ref;
    }

    template<typename T>
    void DynamicRefArray<T>::Resize()
    {
        size_t newSize = m_Count * 2;
        m_RefCounts.resize(newSize, nullptr);

        m_MemoryBlock.Resize(newSize);
        AssignRefCounts();
    }

    template<typename T>
    void DynamicRefArray<T>::AssignRefCounts()
    {
        for (size_t i = 0; i < m_RefCounts.size(); i++)
        {
            const auto& memoryDir = m_MemoryBlock.At(i);
            if (m_RefCounts[i])
                static_cast<RefCountPtr<T>*>(m_RefCounts[i])->m_Pointer = memoryDir;
            else
                m_RefCounts[i] = new RefCountPtr<T>(memoryDir);
        }
    }

    template<typename T, typename Elem>
    constexpr Ref<T> CreateRef(const std::initializer_list<Elem>& initializer_list)
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