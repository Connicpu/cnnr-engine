#pragma once

#include "Component.h"

template <const char *name>
class MarkerComponent : public Component
{
public:
    virtual String GetName() const override
    {
        return name;
    }
};
