#pragma once

#include "RenderBase.h"
#include "RPtr.h"

template <typename T, typename ...Args>
RPtr<T> MakeRenderObject(Args &&...args)
{
    T *object = new T(std::forward<Args>(args)...);
    object->IncRef();
    object->IncWeak();
    return RPtr<T>::FromExisting(object);
}

template <typename Iface, typename Inst>
class ImplRenderBase abstract : public Iface
{
protected:
    RWeak<Inst> inst_;
    inline RPtr<Inst> GetInst()
    {
        RPtr<Inst> inst;
        bool res = inst_.Upgrade(inst);
        assert(res);
        return std::move(inst);
    }

public:
    inline ImplRenderBase(Inst *inst)
        : inst_(inst)
    {
    }
    inline ImplRenderBase(const RWeak<Inst> &inst)
        : inst_(inst)
    {
    }
    inline ImplRenderBase(RWeak<Inst> &&inst)
        : inst_(std::move(inst))
    {
    }

    virtual bool GetInstance(IRenderInstance **instance) override
    {
        RPtr<Inst> ptr;
        bool res = inst_.Upgrade(ptr);
        *instance = ptr.Release();
        return res;
    }
};
