#include "Engine.h"
#include <Common/LibLoading.h>
#include <Common/MathLib.h>
#include <Renderer/Renderer.h>

extern "C" void RunEngine(const EngineOptions &options)
{
    RPtr<IRenderInstance> instance;
    CreateInstParams inst_params;
    inst_params.instance_name = "A kewl engine";
    inst_params.adapter_num = 0;
    CallCreateRenderInstance(*options.backend, &inst_params, &instance);

    RPtr<IDevice> device;
    DeviceParams dev_params;
    dev_params.debug = true;
    instance->CreateDevice(&dev_params, &device);

    RPtr<IDisplay> display;
    DisplayParams disp_params;
    disp_params.device = device.p;
    disp_params.window_title = "asdf";
    instance->CreateDisplay(&disp_params, &display);

    bool quit = false;
    while (!quit)
    {
        auto now = []() { return std::chrono::system_clock::now(); };
        auto poll_time = std::chrono::milliseconds(2);
        auto poll_start = now();

        EventStorage event;
        while (poll_start + poll_time > now() && display->PollEvent(event))
        {
            switch (event.base.type)
            {
                case EventType::Closed:
                    quit = true;
                    break;
            }
        }

        using namespace Math;
        auto color = Color(1, 1, 0);
        display->Clear(&color.r);
        display->Present();
    }
}
