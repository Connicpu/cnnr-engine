#pragma once

#include "Component.h"

template <typename T>
class DataComponent : public T, public Component
{
public:
    DataComponent(const T &data)
        : T(data)
    {
    }
};
