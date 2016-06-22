#pragma once

#include "RenderBase.h"

template <typename T>
class RPtr final
{
private:
    enum NoInc { NoInc_ };

    inline RPtr(T *ptr, NoInc)
        : p(ptr)
    {
    }

    inline void _Assign(T *ptr)
    {
        p = ptr;
        if (p)
            p->IncRef();
    }

public:
    inline RPtr()
        : p(nullptr)
    {
    }

    inline RPtr(T *ptr)
    {
        _Assign(ptr);
    }

    inline RPtr(const RPtr &copy)
    {
        _Assign(copy.p)
    }

    inline RPtr(RPtr &&move)
        : RPtr()
    {
        Swap(move);
    }

    inline ~RPtr()
    {
        Reset();
    }

    inline RPtr &operator=(const RPtr &ptr)
    {
        if (this == std::addressof(ptr))
            return *this;

        Assign(ptr);
        return *this;
    }

    inline RPtr &operator=(RPtr &&ptr)
    {
        if (this == std::addressof(ptr))
            return *this;

        Reset();
        Swap(ptr);
        return *this;
    }

    // Give the p value of this smart pointer and reset it without decrementing the refcount
    inline T *Release()
    {
        auto temp = p;
        p = nullptr;
        return temp;
    }

    // Release this reference to the pointer
    inline void Reset()
    {
        if (p)
        {
            p->DecRef();
            p = nullptr;
        }
    }

    inline void Swap(RPtr &rhs)
    {
        std::swap(p, rhs.p);
    }

    inline void Assign(const RPtr &ptr)
    {
        Reset();
        _Assign(ptr.p);
    }

    inline void Assign(T *ptr)
    {
        Reset();
        _Assign(ptr);
    }

    inline RWeak<T> Downgrade()
    {
        return RWeak<T>{ *this };
    }

    inline T **operator&()
    {
        assert(p == nullptr);
        return &p;
    }

    inline T *operator->() const
    {
        assert(p != nullptr);
        return p;
    }

    inline T &operator*() const
    {
        assert(p != nullptr);
        return *p;
    }

    inline static RPtr FromExisting(T *ptr)
    {
        return RPtr{ ptr, NoInc_ };
    }

    template <typename U>
    inline operator RPtr<U>()
    {
        RPtr<U> copy(p);
        return std::move(copy);
    }

    T *p;
};

template <typename T>
class RWeak final
{
private:
    inline void _Assign(T *ptr)
    {
        p = ptr;
        if (p)
            p->IncWeak();
    }

public:
    inline ~RWeak()
    {
        Reset();
    }

    inline RWeak()
        : p(nullptr)
    {
    }

    explicit inline RWeak(T *ptr)
    {
        _Assign(ptr);
    }

    explicit inline RWeak(const RPtr<T> &ptr)
    {
        _Assign(ptr.p);
    }

    inline RWeak(const RWeak &copy)
    {
        _Assign(copy.p);
    }

    inline RWeak(RWeak &&move)
        : RWeak()
    {
        Swap(move);
    }

    inline RWeak &operator=(const RPtr<T> &ptr)
    {
        Assign(ptr);
        return *this;
    }

    inline RWeak &operator=(const RWeak &copy)
    {
        if (this == std::addressof(copy))
            return *this;

        Assign(copy);
        return *this;
    }

    inline RWeak &operator=(RWeak &&move)
    {
        if (this == std::addressof(move))
            return *this;

        Reset();
        Swap(move);
        return *this;
    }

    inline void Swap(RWeak &rhs)
    {
        std::swap(p, rhs.p);
    }

    inline bool Upgrade(RPtr<T> &ptr)
    {
        if (p && p->TryUpgradeWeak())
        {
            ptr = RPtr<T>::FromExisting(p);
            return true;
        }

        return false;
    }

    inline void Reset()
    {
        if (p)
        {
            p->DecWeak();
            p = nullptr;
        }
    }

    inline void Assign(const RPtr<T> &ptr)
    {
        Reset();
        _Assign(ptr.p);
    }

    inline void Assign(const RWeak &ptr)
    {
        Reset();
        _Assign(ptr.p);
    }

private:
    T *p;
};
