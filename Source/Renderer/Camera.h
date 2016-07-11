#pragma once

#include "RenderBase.h"
#include <Common/MathLib.h>

class ICamera : public IRenderBase
{
public:
    virtual void SetViewport(Size2F viewport) = 0;
    virtual void GetViewport(Size2F *viewport) = 0;
    virtual void SetCenter(Point2F center) = 0;
    virtual void GetCenter(Point2F *center) = 0;

    // The viewport rectangle in world coordinates
    virtual void GetViewRect(RectF *rect) = 0;

    // Transform a point into NDC space
    virtual void TransformPoint(Point2F point, Point2F *out) = 0;
    // Bring an NDC point back into world coordinates
    virtual void UnTransformPoint(Point2F point, Point2F *out) = 0;
};
