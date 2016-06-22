#include <Renderer/Renderer.h>
#include <Common/Platform.h>
#include <AssetPipeline/SpriteLoader.h>
#include <iostream>
#include <Common/MathLib.h>

static const wchar_t RENDERER[] = L"DX11.dll";

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
    using namespace DirectX;

	AllocConsole();
	FILE *f;
	freopen_s(&f, "CONIN$", "r", stdin);
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);

	while (!fs::exists(fs::current_path() / "Assets"))
		fs::current_path(fs::current_path().parent_path());

    HMODULE renderlib = LoadLibraryW(RENDERER);
    PCreateRenderInstance CreateRenderInstance =
        (PCreateRenderInstance)GetProcAddress(renderlib, "CreateRenderInstance");

    CreateInstParams instparams;
    instparams.instance_name = "Test Renderer";
    instparams.adapter_num = 0;
    RPtr<IRenderInstance> inst;
    CreateRenderInstance(&instparams, &inst);

    DeviceParams devparams;
    devparams.debug = true;
    RPtr<IDevice> dev;
    inst->CreateDevice(&devparams, &dev);

    SpriteLoader loader{ dev.p };
    loader.Load("Test");

    DisplayParams disparams;
    disparams.device = dev.p;
    disparams.window_title = "Hi there :D";
    RPtr<IDisplay> display;
    inst->CreateDisplay(&disparams, &display);

	bool quit = false;
    while (!quit)
    {
        EventPtr event;
        while (display->PollEvent(event))
        {
            switch (event->type)
            {
                case EventType::MouseMoved:
                    //std::cout << "Mouse moved!" << std::endl;
                    break;

				case EventType::Closed:
					quit = true;
					break;

                default:
                    std::cout << "Unknown event o:" << std::endl;
                    break;
            }
        }

        static float hue = 0;
        hue = fmodf(hue + 0.01f, 1.0f);
        display->Clear(XMColorHSVToRGB(XMVectorSet(hue, 1, 1, 1)));

        display->Present();
        Sleep(16);
    }
    
    return 0;
}

