#include "pch.h"
#include "DxCamera.h"
#include "DxException.h"

using namespace Math;

DxCamera::DxCamera(DxInstance *inst)
    : ImplRenderBase<ICamera, DxInstance>(inst)
{
    Calculate();
}

void DxCamera::SetViewport(Size2F vp)
{
    if (IsZero(Vec2(vp)) || IsInfinite(Vec2(vp)))
        throw std::runtime_error{ "Zero/Infinity viewport is mathematically erroneous" };

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
    auto topleft =  inv_camera * Point2(-1,  1);
    auto botright = inv_camera * Point2( 1, -1);
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
    changed = true;
    camera_matrix = Matrix3x2::Translation(-center) * Matrix3x2::Scale(2 / viewport);
    if (!camera_matrix.Inverse(inv_camera))
        __fastfail(1);
    // It should be mathematically impossible for this to not be invertible.
    // SetViewport doesn't allow viewport to be zero, infinite, or NaN.
}

void DxCamera::Upload(ID3D11Device *device, ID3D11DeviceContext *context)
{
    if (!changed)
        return;
    changed = false;

    ID3D11Device *dev;
    if (buffer && (buffer->GetDevice(&dev), dev == device))
    {
        D3D11_MAPPED_SUBRESOURCE mapped;
        CheckHR(context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
        memcpy(mapped.pData, static_cast<const Matrix3x2F *>(&camera_matrix), sizeof(Matrix3x2F));
        context->Unmap(buffer, 0);
    }
    else
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = 32;
        desc.StructureByteStride = 32;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = static_cast<const Matrix3x2F *>(&camera_matrix);
        CheckHR(device->CreateBuffer(&desc, &data, &buffer));
    }
}
