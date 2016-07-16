#include "DxDevice.h"
#include "DxException.h"
#include "DxSpriteSet.h"
#include "DxScene.h"
#include "DxCamera.h"
#include "InputLayouts.h"

#ifdef NDEBUG
#define SHADER_FOLDER "Assets/Shaders/DX11/Release/"
#else
#define SHADER_FOLDER "Assets/Shaders/DX11/Debug/"
#endif

DxDevice::DxDevice(DxInstance *inst, const DeviceParams *params)
    : ImplRenderBase<IDevice, DxInstance>(inst)
{
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    if (params->debug)
        flags |= D3D11_CREATE_DEVICE_DEBUG;

    static const D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_10_1,
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

    sprite_shader.Load(device, DxShaderDesc
    {
        SHADER_FOLDER "SpriteVS.cso"_s,
        SHADER_FOLDER "SpritePS.cso"_s,
        { SpriteVSLayout, (int)SpriteVSLayoutCount },
    });
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

void DxDevice::CreateCamera(ICamera **camera)
{
    auto inst = GetInst();
    *camera = MakeRenderObject<DxCamera>(inst.p).Release();
}

void DxDevice::Lock()
{
    AcquireSRWLockExclusive(&lock);
}

void DxDevice::Unlock()
{
    ReleaseSRWLockExclusive(&lock);
}
