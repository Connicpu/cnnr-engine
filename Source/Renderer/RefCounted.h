#pragma once

#include <stdlib.h>
#include <atomic>
#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <memory>

class RefCounted;
template <typename T>
class RPtr;
template <typename T>
class RWeak;

class RefCounted
{
public:
    RefCounted()
        : strong_(0), weak_(0)
    {
    }

    inline void IncRef()
    {
        strong_.fetch_add(1, std::memory_order_relaxed);
    }

    inline void DecRef()
    {
        if (strong_.fetch_sub(1, std::memory_order_release) == 1)
        {
            std::atomic_thread_fence(std::memory_order_acquire);
            Drop();
        }
    }

    inline void IncWeak()
    {
        weak_.fetch_add(1, std::memory_order_relaxed);
    }

    inline void DecWeak()
    {
        if (weak_.fetch_sub(1, std::memory_order_release) == 1)
        {
            std::atomic_thread_fence(std::memory_order_acquire);
            Free();
        }
    }

    inline bool TryUpgradeWeak()
    {
        uint32_t n = strong_.load(std::memory_order_relaxed);
        while (n != 0)
        {
            if (strong_.compare_exchange_weak(n, n + 1, std::memory_order_relaxed, std::memory_order_relaxed))
            {
                return true;
            }
        }
        return false;
    }

    inline uint32_t GetRefCount(std::memory_order mem_order = std::memory_order_seq_cst)
    {
        return strong_.load(mem_order);
    }

protected:
    std::atomic<uint32_t> strong_;
    std::atomic<uint32_t> weak_;
    inline virtual ~RefCounted() {}

    inline void Drop()
    {
        this->~RefCounted();

        if (weak_.fetch_sub(1, std::memory_order_release) == 1)
        {
            std::atomic_thread_fence(std::memory_order_acquire);
            Free();
        }
    }

    virtual void Free()
    {
#ifdef _WIN32
        _aligned_free(this);
#else
        free(this);
#endif
    }

    inline static void *operator new(size_t size)
    {
        void *ptr;
#ifdef _WIN32
        ptr = _aligned_malloc(size, 16);
#else
        ptr = aligned_alloc(16, size);
#endif
        if (!ptr)
            throw std::bad_alloc{};
        return ptr;
    }

    inline static void operator delete(void *ptr)
    {
#ifdef _WIN32
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }

    template <typename T, typename ...Args>
    friend RPtr<T> MakeRenderObject(Args &&...args);
};


