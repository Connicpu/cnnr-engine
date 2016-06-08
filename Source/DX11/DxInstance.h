#pragma once

#include <Renderer/Instance.h>
#include "DxPlatform.h"

void CreateRenderInstance(const CreateInstParams *params, IRenderInstance **inst);

class DxInstance : public IRenderInstance
{
public:
    DxInstance(const CreateInstParams *params);

    virtual void CreateDevice(const DeviceParams *params, IDevice **device) override;
    virtual void CreateDisplay(const DisplayParams *params, IDisplay **display) override;

    ComPtr<IDXGIFactory2> dxgi_factory;
    ComPtr<IDXGIAdapter1> dxgi_adapter;
};
