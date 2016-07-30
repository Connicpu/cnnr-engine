#pragma once

#include <memory>

template <typename T>
struct MaybeDeleter
{
    MaybeDeleter(bool owned = true)
        : owned(owned)
    {
    }

    bool owned;

    void operator()(T *p)
    {
        if (owned)
        {
            delete p;
        }
    }
};

template <typename T>
using MaybeOwned = std::unique_ptr<T, MaybeDeleter<T>>;
