#include <Renderer/Renderer.h>
#include <Common/Platform.h>
#include <AssetPipeline/SpriteLoader.h>

static const wchar_t RENDERER[] = L"DX11.dll";

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
    HMODULE renderlib = LoadLibraryW(RENDERER);
    PCreateRenderInstance CreateRenderInstance =
        (PCreateRenderInstance)GetProcAddress(renderlib, "CreateRenderInstance");

    CreateInstParams instparams;
    instparams.instance_name = "Test Renderer";
    instparams.adapter_num = 0;
    RPtr<IRenderInstance> inst;
    CreateRenderInstance(&instparams, &inst);

    DeviceParams devparams;
    devparams.debug = false;
    RPtr<IDevice> dev;
    inst->CreateDevice(&devparams, &dev);

    SpriteLoader loader{ dev.p };
    loader.Load("Test");
    
    return 0;
}

