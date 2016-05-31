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
