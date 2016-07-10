#pragma once

#include <Renderer/Camera.h>
#include <Renderer/RenderHelpers.h>
#include "DxInstance.h"

class DxCamera : public ImplRenderBase<ICamera, DxInstance>
{
public:
    virtual void SetViewport(Size2F viewport) override;
    virtual void GetViewport(Size2F *viewport) override;
    virtual void SetCenter(Point2F center) override;
    virtual void GetCenter(Point2F *center) override;

    // The viewport rectangle in world coordinates
    virtual void GetViewRect(RectF *rect) override;

    // Transform a point into NDC space
    virtual void TransformPoint(Point2F point, Point2F *out) override;
    // Bring an NDC point back into world coordinates
    virtual void UnTransformPoint(Point2F point, Point2F *out) override;

    void Calculate();

    Size2F viewport = Math::SizeF(1, 1);
    Point2F center = Math::Point2();
    Math::Matrix3x2 camera_matrix = Math::Matrix3x2::Identity();
    Math::Matrix3x2 inv_camera = Math::Matrix3x2::Identity();
};

