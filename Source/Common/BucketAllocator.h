#pragma once

#include <utility>
#include <algorithm>
#include <vector>
#include <memory>
#include <stdint.h>

struct BucketAllocation
{
    void *memory;
    void *ref;
};

template <size_t Size, size_t Align, size_t ElemsPer>
class BucketAllocator
{
public:
    BucketAllocation Alloc();
    void Free(BucketAllocation alloc);

private:
    struct Bucket
    {
        size_t free_count = ElemsPer;
        size_t free_stack_len = 0;
        size_t free_stack[ElemsPer];
        std::aligned_storage_t<Size, Align> elements[ElemsPer];

        void *operator new(size_t size);
        void operator delete(void *ptr);
    };

    std::vector<std::unique_ptr<Bucket>> buckets;
    std::vector<Bucket *> free_buckets;
};

template <typename T, size_t ElemsPer>
using TBucketAllocator = BucketAllocator<sizeof(T), alignof(T), ElemsPer>;

template<size_t Size, size_t Align, size_t ElemsPer>
inline BucketAllocation BucketAllocator<Size, Align, ElemsPer>::Alloc()
{
    static_assert(ElemsPer > 1, "ElemsPer must be greater than 1");

    Bucket *bucket;
    if (free_buckets.empty())
    {
        std::unique_ptr<Bucket> new_bucket = std::make_unique<Bucket>();
        bucket = new_bucket.get();
        free_buckets.push_back(bucket);
        buckets.push_back(std::move(new_bucket));
    }
    else
    {
        bucket = free_buckets.back();
        if (bucket->free_count == 1)
            free_buckets.pop_back();
    }

    size_t id;
    if (bucket->free_stack_len)
    {
        id = bucket->free_stack[--bucket->free_stack_len];
    }
    else
    {
        id = ElemsPer - bucket->free_count;
    }

    bucket->free_count--;

    BucketAllocation alloc;
    alloc.memory = &bucket->elements[id];
    alloc.ref = bucket;
    return alloc;
}

template<size_t Size, size_t Align, size_t ElemsPer>
inline void BucketAllocator<Size, Align, ElemsPer>::Free(BucketAllocation alloc)
{
    auto *elem = (std::aligned_storage_t<Size, Align> *)alloc.memory;
    Bucket *bucket = (Bucket *)alloc.ref;
    if (bucket->free_count == 0)
        free_buckets.push_back(bucket);

    size_t id = elem - &bucket->elements[0];
    bucket->free_stack[bucket->free_stack_len++] = id;
    bucket->free_count++;
}

template<size_t Size, size_t Align, size_t ElemsPer>
inline void *BucketAllocator<Size, Align, ElemsPer>::Bucket::operator new(size_t size)
{
#ifdef _WIN32
    if (auto ptr = _aligned_malloc(size, alignof(Bucket)))
#else
    if (auto ptr = aligned_alloc(alignof(Bucket), size))
#endif
        return ptr;
    throw std::bad_alloc{};
}

template<size_t Size, size_t Align, size_t ElemsPer>
inline void BucketAllocator<Size, Align, ElemsPer>::Bucket::operator delete(void *ptr)
{
#ifdef _WIN32
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

