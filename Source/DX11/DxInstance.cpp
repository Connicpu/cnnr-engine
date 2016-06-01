#include "DxInstance.h"
#include <Renderer/RenderHelpers.h>

void CreateRenderInstance(const CreateInstParams *params, IRenderInstance **inst)
{
    RPtr<DxInstance> ptr = MakeRenderObject<DxInstance>();
    *inst = ptr.Release();
}

void DxInstance::CreateDisplay(IDevice *device, IDisplay **display)
{
    
}
