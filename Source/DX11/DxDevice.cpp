#include "DxDevice.h"
#include "DxException.h"
#include "SpriteSet.h"

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

void DxDevice::CreateSpriteSet(
    bool streaming, uint32_t spriteCount,
    uint32_t spriteWidth, uint32_t spriteHeight,
    const uint32_t **buffers,
    ISpriteSet **set)
{
    if (!streaming) assert(buffers);
    auto inst = GetInst();

    *set = MakeRenderObject<SpriteSet>(
        inst.p, this, streaming, spriteCount, spriteWidth,
        spriteHeight, buffers
    ).Release();
}
