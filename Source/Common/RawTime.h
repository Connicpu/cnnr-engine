#pragma once

#include <stdint.h>
#include "Platform.h"
#include "optional.h"

namespace rawtime_details
{
    const uint64_t NSEC_PER_SEC = 1000000000;
}

inline uint64_t rawtime_resolution()
{
#ifdef WIN32
    LARGE_INTEGER res;
    QueryPerformanceFrequency(&res);
    return (uint64_t)res.QuadPart;
#else
    return rawtime_details::NSEC_PER_SEC;
#endif
}

inline uint64_t rawtime_now()
{
#if defined(WIN32)

    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return (uint64_t)time.QuadPart;

#elif defined(MACOS)

    static std::optional<clock_serv_t> cclock;
    mach_timespec_t time;

    if (!cclock)
    {
        clock_serv_t cclock_temp;
        host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock_temp);
        cclock = cclock_temp;
    }

    if (clock_get_time(*cclock, &time) != 0)
        throw std::runtime_error{ "clock_get_time(SYSTEM_CLOCK) failed" };

    return uint64_t(time.tv_nsec) * rawtime_details::NSEC_PER_SEC + uint64_t(time.tv_sec);

#else

    timespec time;
    if (clock_gettime(CLOCK_MONOTONIC, &time) != 0)
        throw std::runtime_error{ "clock_gettime(CLOCK_MONOTONIC) failed" };
    return uint64_t(time.tv_nsec) * rawtime_details::NSEC_PER_SEC + uint64_t(time.tv_sec);

#endif
}

