#pragma once

#include "RenderBase.h"
#include <Common/MathLib.h>

class IRenderTarget : public IRenderBase
{
public:
    virtual void Clear(float color[4]) = 0;
    // For internal use only
    virtual void GetTargetObject(void *target) = 0;

    inline void Clear(Math::FXMVECTOR color)
    {

    }
};
