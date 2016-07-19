#pragma once

#include "RenderBase.h"
#include <Common/MathLib.h>

class ICamera : public IRenderBase
{
public:
    virtual void SetViewport(Math::Size2F viewport) = 0;
    virtual void GetViewport(Math::Size2F *viewport) = 0;
    virtual void SetCenter(Math::Point2F center) = 0;
    virtual void GetCenter(Math::Point2F *center) = 0;

    // The viewport rectangle in world coordinates
    virtual void GetViewRect(Math::RectF *rect) = 0;

    // Transform a point into NDC space
    virtual void TransformPoint(Math::Point2F point, Math::Point2F *out) = 0;
    // Bring an NDC point back into world coordinates
    virtual void UnTransformPoint(Math::Point2F point, Math::Point2F *out) = 0;
};
