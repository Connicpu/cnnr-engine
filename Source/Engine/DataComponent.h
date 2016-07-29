#pragma once

#include "Component.h"

template <typename T>
class DataComponent : public T, public Component
{
public:
    DataComponent(String component_name, const T &data)
        : T(data), name_(std::move(component_name))
    {
    }

    DataComponent(String component_name, T &&data)
        : T(data), name_(std::move(component_name))
    {
    }

    virtual String GetName() const override
    {
        return name_.span();
    }

private:
    String name_;
};
