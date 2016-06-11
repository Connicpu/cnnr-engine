#include "DxDevice.h"
#include "DxException.h"

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
    ComPtr<ID3D11Device> d3d_device;
    ComPtr<ID3D11DeviceContext> d3d_context;
    CheckHR(D3D11CreateDevice(
        inst->dxgi_adapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        flags,
        feature_levels,
        ARRAYSIZE(feature_levels),
        D3D11_SDK_VERSION,
        &d3d_device,
        &feature_level,
        &d3d_context
    ));
}
