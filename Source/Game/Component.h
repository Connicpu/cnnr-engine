#pragma once

#include <memory>

class Component
{
public:
    virtual ~Component() {}
};

using ComponentPtr = std::unique_ptr<Component>;

template <typename T>
using ComponentPtrT = std::unique_ptr<T>;
