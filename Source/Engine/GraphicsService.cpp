#include "pch.h"
#include "GraphicsService.h"

GraphicsService::GraphicsService(const DynamicLibrary &backend)
{
    CreateInstParams inst_params;
    inst_params.adapter_num = 0;
    inst_params.instance_name = "CnnrEngine";
    backend.Call<PFCreateRenderInstance>("CreateRenderInstance", &inst_params, &instance_);
}
