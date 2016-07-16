#pragma once

#include "RenderBase.h"

class IScene;
class ICamera;

class IRenderTarget : public IRenderBase
{
public:
    virtual void BeginDraw() = 0;
    virtual void Clear(const float color[4]) = 0;
    // For internal use only
    virtual void GetTargetObject(void *target) = 0;

    virtual void DrawScene(
        IScene *scene,
        ICamera *camera
    ) = 0;
};
