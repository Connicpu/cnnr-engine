#pragma once

#ifdef _WIN32

#include "Platform.h"
#include <atomic>

class RunOnce
{
public:
    RunOnce() = default;
    RunOnce(const RunOnce &) = delete;
    RunOnce &operator=(const RunOnce &) = delete;

    template <typename F>
    void Once(F &&f)
    {
        AcquireSRWLockShared(&lock);
        auto init = initialized.load(std::memory_order_acquire);
        ReleaseSRWLockShared(&lock);
        if (init) return;

        AcquireSRWLockExclusive(&lock);
        auto init = initialized.load(std::memory_order_acquire);
        if (!init)
        {
            f();

            initialized.store(true, std::memory_order_release);
        }
        ReleaseSRWLockExclusive(&lock);
    }

private:
    SRWLOCK lock = SRWLOCK_INIT;
    std::atomic<bool> initialized = false;
};

#else

#include <pthread.h>
#include <atomic>

class RunOnce
{
public:
    RunOnce() = default;
    RunOnce(const RunOnce &) = delete;
    RunOnce &operator=(const RunOnce &) = delete;

    template <typename F>
    void Once(F &&f)
    {
        pthread_rwlock_rdlock(&lock);
        auto init = initialized.load(std::memory_order_acquire);
        pthread_rwlock_unlock(&lock);
        if (init) return;

        pthread_rwlock_wrlock(&lock);
        auto init = initialized.load(std::memory_order_acquire);
        if (!init)
        {
            f();

            initialized.store(true, std::memory_order_release);
        }
        pthread_rwlock_unlock(&lock);
    }

private:
    pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
    std::atomic<bool> initialized = false;
};

#endif
