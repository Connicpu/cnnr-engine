#include "DxDevice.h"
#include "DxException.h"
#include "DxSpriteSet.h"
#include "DxScene.h"

DxDevice::DxDevice(DxInstance *inst, const DeviceParams *params)
    : ImplRenderBase<IDevice, DxInstance>(inst)
{
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    if (params->debug)
        flags |= D3D11_CREATE_DEVICE_DEBUG;

    static const D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_10_0,
    };

    D3D_FEATURE_LEVEL feature_level;
    CheckHR(D3D11CreateDevice(
        inst->dxgi_adapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        flags,
        feature_levels,
        ARRAYSIZE(feature_levels),
        D3D11_SDK_VERSION,
        &device,
        &feature_level,
        &context
    ));
}

void DxDevice::CreateSpriteSet(const SpriteSetParams *params, ISpriteSet **set)
{
    if (!params->streaming) assert(params->buffers);
    auto inst = GetInst();

    *set = MakeRenderObject<DxSpriteSet>(
        inst.p, this, params->streaming, params->sprite_count, params->sprite_width,
        params->sprite_height, params->buffers
    ).Release();
}

void DxDevice::CreateScene(IScene **scene)
{
    auto inst = GetInst();
    *scene = MakeRenderObject<DxScene>(inst.p, Math::SizeF(20, 20)).Release();
}
