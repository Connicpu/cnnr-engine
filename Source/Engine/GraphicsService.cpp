#include "pch.h"
#include "GraphicsService.h"

GraphicsService::GraphicsService(const DynamicLibrary &backend)
{
    CreateInstParams inst_params;
    inst_params.adapter_num = 0;
    inst_params.instance_name = "CnnrEngine";
    backend.Call<PFCreateRenderInstance>("CreateRenderInstance", &inst_params, &instance_);

    DeviceParams dev_params;
    dev_params.debug = true;
    instance_->CreateDevice(&dev_params, &device_);

    DisplayParams disp_params;
    disp_params.device = device_.p;
    disp_params.window_title = "Dat window";
    instance_->CreateDisplay(&disp_params, &display_);
}

IRenderInstance * GraphicsService::GetInstance() const
{
    return instance_.p;
}

IDevice * GraphicsService::GetDevice() const
{
    return device_.p;
}

IDisplay * GraphicsService::GetDisplay() const
{
    return display_.p;
}
