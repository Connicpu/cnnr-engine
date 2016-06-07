#pragma once

#include "RenderBase.h"

class IRenderTarget : public IRenderBase
{
public:
    virtual void Clear(float color[4]) = 0;

#ifndef RENDERER_INTERNALS
protected:
#endif
    virtual void GetTargetObject(void *target) = 0;
};
