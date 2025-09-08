#pragma once

#include "OpenEngine/Core/Includes.h"

namespace OpenGraphics
{
    template <typename T, size_t N>
    struct OPEN_API MemoryBuffer
    {
    public:
        T* At(size_t index);
        const T* At(size_t index) const;
        T& operator[](const size_t index) { return *At(index); }
        T operator[](const size_t index) const { return *At(index); }

    private:
        alignas(T) unsigned char m_MemoryBlock[N * sizeof(T)] = {};
    };

    template <typename T>
    struct OPEN_API UnboundedMemoryBuffer
    {
    public:
        explicit UnboundedMemoryBuffer(const size_t N) :
            m_Size(std::max(1ull, N)),
            m_MemoryBlock(new unsigned char[std::max(1ull, N) * sizeof(T)]) {}
        ~UnboundedMemoryBuffer() { delete[] m_MemoryBlock; }

        T* At(size_t index);
        const T* At(size_t index) const;
        T& operator[](const size_t index) { return *At(index); }
        T operator[](const size_t index) const { return *At(index); }

        [[nodiscard]] size_t Size() const { return m_Size; }

        void Resize(size_t newSize);

    private:
        size_t m_Size = 0;
        alignas(T) unsigned char* m_MemoryBlock = nullptr;
    };

    struct RefCountBase
    {
    protected:
        inline void Increase() { ++m_RefCount; }
        inline void Decrease() { --m_RefCount; }
        [[nodiscard]] inline uint32_t GetReferenceCount() const { return m_RefCount.load(); }

        volatile std::atomic_uint32_t m_RefCount = 0;

        template<typename T>
        friend class Ref;

        template<typename T, size_t N>
        friend class RefArray;

        template<typename T>
        friend class DynamicRefArray;
    };

    template <typename T>
    struct RefCountPtr : public RefCountBase
    {
    public:
        RefCountPtr() = default;
        RefCountPtr(std::nullptr_t) : m_Pointer(nullptr) {}
        RefCountPtr(T* pointer) : m_Pointer(pointer) {}
        template <typename... Args>
        RefCountPtr(Args&&... args) : m_Pointer(new T(std::forward<Args>(args)...)) {}
        ~RefCountPtr() = default;

    private:
        T* m_Pointer = nullptr;

        // HACK: Not pretty, but it works
        template<typename T0>
        friend class DynamicRefArray;
        template<typename T0>
        friend class Ref;

        template<typename T0, typename Elem>
        friend constexpr Ref<T0> CreateRef(const std::initializer_list<Elem>& initializer_list);

        template<typename T0>
        friend constexpr Ref<T0> CreateRef(const Ref<T0>& reference);

        template<typename T0, typename... Args>
        friend constexpr Ref<T0> CreateRef(Args&&... args);
    };

    template<typename T>
    class OPEN_API Ref
    {
    public:
        Ref() = default;
        Ref(std::nullptr_t) : m_Pointer(nullptr) {}
        Ref(T* pointer) : m_Pointer(pointer), m_RefCount(new RefCountPtr<T>(pointer))
        {
            IncreaseRefCount();
        }

        Ref(const Ref& other)
            : m_Pointer(other.m_Pointer), m_RefCount(other.m_RefCount), m_OwnsRefCount(other.m_OwnsRefCount)
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

        T* operator->() { return Get(); }
        const T* operator->() const { return Get(); }

        T& operator*() { return *Get(); }
        const T& operator*() const { return *Get(); }

        T* Get() { Update(); return m_Pointer; }
        const T* Get() const { Update(); return m_Pointer; }

    private:
        Ref(T* pointer, RefCountBase* refCount) : m_Pointer(pointer), m_RefCount(refCount)
        {
            IncreaseRefCount();
        }

        void IncreaseRefCount() const;
        void DecreaseRefCount() const;
        void Update() const;

        mutable T* m_Pointer = nullptr;
        mutable RefCountBase* m_RefCount = nullptr;
        bool m_OwnsRefCount = true;

        template<typename T0, size_t N>
        friend class RefArray;

        template<typename T0>
        friend class DynamicRefArray;

        template<typename T0, typename Elem>
        friend constexpr Ref<T0> CreateRef(const std::initializer_list<Elem>& initializer_list);

        template<typename T0>
        friend constexpr Ref<T0> CreateRef(const Ref<T0>& reference);

        template<typename T0, typename... Args>
        friend constexpr Ref<T0> CreateRef(Args&&... args);
    };

    template<typename T, size_t N>
    class OPEN_API RefArray
    {
    public:
        RefArray()
        {
            AssignRefCounts();
        }

        template<typename... Args>
        constexpr Ref<T> CreateRef(size_t index, Args&&... args);

    private:
        void AssignRefCounts();

        MemoryBuffer<T, N> m_MemoryBlock;
        MemoryBuffer<RefCountPtr<T>, N> m_RefCounts;
    };

    template<typename T>
    class OPEN_API DynamicRefArray
    {
    public:
        explicit DynamicRefArray(size_t count = 10);

        template<typename... Args>
        constexpr void EmplaceNew(Args&&... args);

        constexpr Ref<T> CreateRefAt(size_t index);
        constexpr Ref<T> CreateRefLast();

        void Resize();

    private:
        void AssignRefCounts();

        UnboundedMemoryBuffer<T> m_MemoryBlock;
        std::vector<RefCountBase*> m_RefCounts;
        std::size_t m_Count = 0;
    };

    template <typename T>
    using BorrowRef = const Ref<T>&;
}

#include "Memory.inl"
