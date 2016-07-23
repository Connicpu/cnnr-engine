#pragma once

#include <cassert>
#include <vector>

template <typename T, typename U>
inline T *checked_cast(U *ptr)
{
#ifdef NDEBUG
    return static_cast<T *>(ptr);
#else
    T *new_ptr = dynamic_cast<T *>(ptr);
    assert(new_ptr);
    return new_ptr;
#endif
}
