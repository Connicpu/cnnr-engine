#pragma once

#include "RenderBase.h"

class IRenderTarget : public IRenderBase
{
public:
    virtual void BeginDraw() = 0;
    virtual void Clear(float color[4]) = 0;
    // For internal use only
    virtual void GetTargetObject(void *target) = 0;
};
