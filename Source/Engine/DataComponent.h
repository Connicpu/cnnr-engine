#pragma once

#include "Component.h"

template <typename T>
class DataComponent : public T, public Component
{
public:
    DataComponent() = default;

    DataComponent(const T &data)
        : T(data)
    {
    }

    DataComponent(T &&data)
        : T(data)
    {
    }

    virtual String GetName() const override
    {
        return component_name<T>::value;
    }
};
