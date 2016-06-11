#pragma once

#include <Renderer/Device.h>
#include <Renderer/RenderHelpers.h>
#include "DxInstance.h"

class DxDevice : public ImplRenderBase<IDevice, DxInstance>
{
public:
    DxDevice(DxInstance *inst, const DeviceParams *params);
};
