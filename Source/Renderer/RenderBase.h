#pragma once

#include "RefCounted.h"

class IRenderInstance;

class IRenderBase : public RefCounted
{
public:
    virtual bool GetInstance(IRenderInstance **instance) = 0;
};


