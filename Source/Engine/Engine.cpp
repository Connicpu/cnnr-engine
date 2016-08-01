#include "Engine.h"
#include "GameData.h"
#include "Transform.h"
#include <Common/LibLoading.h>
#include <Common/MathLib.h>
#include <Renderer/Renderer.h>
#include <LuaInterface/LuaState.h>

void Engine(const EngineOptions &options)
{
    using namespace Math;

    LuaState state;
    state.load("test");

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

    GameData data;

    auto e1 = data.entities.CreateEntity();
    data.components.transform.insert(e1);
    data.components.transform[e1].rotation = Rads(45_deg);

    auto e2 = data.entities.CreateEntity();
    data.components.transform.insert(e2);
    data.components.transform[e2].parent = e1;
    data.components.transform[e2].position = Math::Point2(1, 0);

    data.entities.FlushQueue(data);

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

        data.systems.Process(data);
        data.entities.FlushQueue(data);

        using namespace Math;
        auto color = Color(1, 1, 0);
        display->Clear(&color.r);
        display->Present();
    }
}

extern "C" const char *RunEngine(const EngineOptions &options)
{
    try
    {
        Engine(options);
    }
    catch (const std::exception &ex)
    {
        size_t len = strlen(ex.what());
        char *what = new char[len + 1];
        memcpy(what, ex.what(), len);
        what[len] = 0;
        return what;
    }

    return nullptr;
}
