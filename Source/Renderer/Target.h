#pragma once

#include "RenderBase.h"
#include <Common/MathLib.h>

class IRenderTarget : public IRenderBase
{
public:
    virtual void BeginDraw() = 0;
    virtual void Clear(float color[4]) = 0;
    // For internal use only
    virtual void GetTargetObject(void *target) = 0;

    inline void Clear(Math::FXMVECTOR color)
    {
        Math::XMFLOAT4 clear;
        Math::XMStoreFloat4(&clear, color);
        this->Clear(&clear.x);
    }
};
