#pragma once

#include <Common/LibLoading.h>
#include <Renderer/Renderer.h>

class GraphicsService
{
public:
    GraphicsService(const DynamicLibrary &backend);

private:
    RPtr<IRenderInstance> instance_;
};
