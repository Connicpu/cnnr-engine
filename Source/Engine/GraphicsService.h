#pragma once

#include <Common/LibLoading.h>
#include <Renderer/Renderer.h>

class GraphicsService
{
public:
    GraphicsService(const DynamicLibrary &backend);

    IRenderInstance *GetInstance() const;
    IDevice *GetDevice() const;
    IDisplay *GetDisplay() const;

private:
    RPtr<IRenderInstance> instance_;
    RPtr<IDevice> device_;
    RPtr<IDisplay> display_;
};
