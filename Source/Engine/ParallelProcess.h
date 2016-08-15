#pragma once

#include "System.h"
#include "Entity.h"
#include "GameData.h"
#include <thread>
#include <future>
#include <algorithm>

#ifdef aMSVC

#include <ppl.h>

template <typename Func>
inline void ParallelProcess(GameData &data, const Func &f)
{
    concurrency::parallel_for(0u, data.entities.MaxIndex(), [&data, &f](uint32_t idx)
    {
        if (auto entity = data.entities.EntityAtIndex(idx))
            f(data, *entity);
    });
}

#else

template <typename Func>
inline void ParallelProcess(GameData &data, const Func &f)
{
    const uint32_t PARALLEL_THRESHOLD = 1024*5;

    auto max = data.entities.MaxIndex();
    if (max < PARALLEL_THRESHOLD * 2)
    {
        for (uint32_t i = 0; i < max; ++i)
        {
            if (auto entity = data.entities.EntityAtIndex(i))
                f(data, *entity);
        }
    }
    else
    {
        std::vector<std::future<void>> handles;
        auto max_threads = std::max(std::min(max / PARALLEL_THRESHOLD, std::thread::hardware_concurrency()*2), 1u);
        auto e_per_thread = max / max_threads;
        auto excess = max - (e_per_thread * max_threads);

        handles.reserve(max_threads);

        for (unsigned t = 0; t < max_threads; ++t)
        {
            auto start = e_per_thread * t;
            auto num = e_per_thread;
            if (t == max_threads - 1)
                num += excess;

            handles.push_back(std::async([&data, &f, start, num]()
            {
                for (uint32_t i = 0; i < num; ++i)
                {
                    if (auto entity = data.entities.EntityAtIndex(start + i))
                        f(data, *entity);
                }
            }));
        }

        for (auto &handle : handles)
        {
            handle.get();
        }
    }
}

#endif
