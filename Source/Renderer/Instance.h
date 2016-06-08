#pragma once

#include "RefCounted.h"

class IDisplay;
class IDevice;
class IRenderInstance;

struct CreateInstParams;
struct DeviceParams;
struct DisplayParams;

using PCreateRenderInstance = void(*)(const CreateInstParams *params, IRenderInstance **inst);

class IRenderInstance : public RefCounted
{
public:
    virtual void CreateDevice(const DeviceParams *params, IDevice **device) = 0;
    virtual void CreateDisplay(const DisplayParams *params, IDisplay **display) = 0;
};

struct CreateInstParams
{
    const char *instance_name;
    uint32_t adapter_num;
};

struct DeviceParams
{
    bool debug;
};

struct DisplayParams
{
    IDevice *device;
    const char *window_title;
};
