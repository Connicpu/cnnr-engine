#pragma once

#include "RefCounted.h"

class IDisplay;
class IDevice;
class IRenderInstance;
struct CreateInstParams;

using PCreateRenderInstance = void(*)(const CreateInstParams *params, IRenderInstance **inst);

class IRenderInstance : public RefCounted
{
public:
    virtual void CreateDisplay(IDevice *device, IDisplay **display) = 0;
};
