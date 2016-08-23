#pragma once

#include <stdint.h>
#include <utility>

namespace cx
{
    namespace details
    {
        template <typename T, T offset, T prime>
        constexpr T fnv1a_helper(const char *str, ptrdiff_t pos)
        {
            return pos == 0
                ? offset
                : (fnv1a_helper<T, offset, prime>(str, pos - 1) ^ str[pos - 1]) * prime;
        }
    }

    constexpr uint32_t fnv1a_32(const char *str, size_t len)
    {
        return details::fnv1a_helper<uint32_t, 2166136261U, 16777619U>(str, (ptrdiff_t)len);
    }

    constexpr uint64_t fnv1a_64(const char *str, size_t len)
    {
        return details::fnv1a_helper<uint64_t, 14695981039346656037U, 1099511628211U>(str, (ptrdiff_t)len);
    }

    constexpr size_t fnv1a(const char *str, size_t len)
    {
        return std::is_same<size_t, uint32_t>::value
            ? (size_t)fnv1a_32(str, len)
            : (size_t)fnv1a_64(str, len);
    }

    constexpr uint32_t operator""_fnv1a_32(const char *str, size_t len)
    {
        return fnv1a_32(str, len);
    }
    constexpr uint64_t operator""_fnv1a_64(const char *str, size_t len)
    {
        return fnv1a_64(str, len);
    }
    constexpr size_t operator""_fnv1a(const char *str, size_t len)
    {
        return fnv1a(str, len);
    }
}
