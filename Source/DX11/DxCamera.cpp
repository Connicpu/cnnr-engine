#include "DxCamera.h"

using namespace Math;

void DxCamera::SetViewport(Size2F vp)
{
    if (IsZero(Vec2(vp)))
        throw std::runtime_error{ "Zero viewport is mathematically erroneous" };

    viewport = vp;
    Calculate();
}

void DxCamera::GetViewport(Size2F *vp)
{
    *vp = viewport;
}

void DxCamera::SetCenter(Point2F c)
{
    center = c;
    Calculate();
}

void DxCamera::GetCenter(Point2F *c)
{
    *c = center;
}

void DxCamera::GetViewRect(RectF *rect)
{
    auto topleft = inv_camera * Point2(-1, 1);
    auto botright = inv_camera * Point2(1, -1);
    rect->left = topleft.x;
    rect->top = topleft.y;
    rect->right = botright.x;
    rect->bottom = botright.y;
}

void DxCamera::TransformPoint(Point2F point, Point2F *out)
{
    *out = camera_matrix * point;
}

void DxCamera::UnTransformPoint(Point2F point, Point2F *out)
{
    *out = inv_camera * point;
}

void DxCamera::Calculate()
{
    camera_matrix = Matrix3x2::Translation(-center) * Matrix3x2::Scale(2 / viewport);
    if (!camera_matrix.Inverse(inv_camera))
        __fastfail(1); // It should be mathematically impossible for this to not be invertible
}
