#pragma once

#include <Renderer/Device.h>
#include <Renderer/RenderHelpers.h>
#include "DxInstance.h"

class DxDevice : public ImplRenderBase<IDevice, DxInstance>
{
public:
    DxDevice(DxInstance *inst, const DeviceParams *params);

    virtual void CreateSpriteSet(
        bool streaming, uint32_t spriteCount,
        uint32_t spriteWidth, uint32_t spriteHeight,
        const uint32_t **buffers,
        ISpriteSet **set
    ) override;

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
};
