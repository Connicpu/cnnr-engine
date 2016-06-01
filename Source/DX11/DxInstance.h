#pragma once

#include <Renderer/Instance.h>

void CreateRenderInstance(const CreateInstParams *params, IRenderInstance **inst);

class DxInstance : public IRenderInstance
{
public:
    virtual void CreateDisplay(IDevice *device, IDisplay **display) override;
};
