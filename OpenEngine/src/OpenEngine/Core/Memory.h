#pragma once

#include "OpenEngine/Core/Includes.h"

namespace OpenGraphics
{
    struct RefCount
    {
    private:
        inline void Increase() { ++m_RefCount; }
        inline void Decrease() { --m_RefCount; }
        inline uint32_t GetReferenceCount() const { return m_RefCount.load(); }

        mutable volatile std::atomic_uint32_t m_RefCount = 0;

        template<typename T>
        friend class Ref;
    };

    template<typename T>
    class OPEN_API Ref
    {
    public:
        Ref() = default;
        Ref(std::nullptr_t) : m_Pointer(nullptr) {}
        Ref(T* pointer) : m_Pointer(pointer), m_RefCount(new RefCount)
        {
            IncreaseRefCount();
        }

        Ref(const Ref& other) : m_Pointer(other.m_Pointer), m_RefCount(other.m_RefCount)
        {
            IncreaseRefCount();
        }

        ~Ref()
        {
            DecreaseRefCount();
        }

        Ref& operator=(std::nullptr_t)
        {
            DecreaseRefCount();
            m_Pointer = nullptr;
            return *this;
        }

        Ref& operator=(const Ref& other)
        {
            DecreaseRefCount();
            m_Pointer = other.m_Pointer;
            m_RefCount = other.m_RefCount;
            IncreaseRefCount();
            return *this;
        }

        operator bool() { return m_Pointer != nullptr; }
        operator bool() const { return m_Pointer != nullptr; }

        T* operator->() { return m_Pointer; }
        const T* operator->() const { return m_Pointer; }

        T& operator*() { return *m_Pointer; }
        const T& operator*() const { return *m_Pointer; }

        T* Get() { return m_Pointer; }
        const T* Get() const { return m_Pointer; }

    private:
        void IncreaseRefCount() const
        {
            if (m_Pointer)
                m_RefCount->Increase();
        }

        void DecreaseRefCount() const
        {
            if (!m_RefCount) return;

            m_RefCount->Decrease();
            if (m_RefCount->GetReferenceCount() == 0)
            {
                delete m_RefCount;
                m_RefCount = nullptr;
                if (m_Pointer)
                {
                    delete m_Pointer;
                    m_Pointer = nullptr;
                }
            }
        }

        mutable T* m_Pointer = nullptr;
        mutable RefCount* m_RefCount = nullptr;
    };

    template<typename T, typename Elem>
    constexpr Ref<T> CreateRef(const std::initializer_list<Elem> initializer_list)
    {
        return Ref<T>(new T(initializer_list));
    }

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        // NOTE: we should reduce the number of allocations to one
        return Ref<T>(new T(std::forward<Args>(args)...));
    }

    template <typename T>
    using BorrowRef = const Ref<T>&;
}