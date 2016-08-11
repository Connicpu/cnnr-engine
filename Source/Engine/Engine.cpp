#include "pch.h"
#include "Engine.h"
#include "GameData.h"
#include "Transform.h"
#include "LuaModules.h"
#include <Common/LibLoading.h>
#include <Common/MathLib.h>
#include <Renderer/Renderer.h>
#include <LuaInterface/LuaState.h>
#include <LuaInterface/ModulePack.h>
#include <connorlib/serialization/toml.h>

extern "C" void RunEngine(const EngineOptions &options)
{
    // Create the lua state
    LuaState state;
    RegisterLuaModules(state);

    // Initialize the game data
    GameData data(state);

    // Open the graphics system
    data.services.graphics = GraphicsService(*options.backend);

    // Testing
    auto start = std::chrono::system_clock::now();
    state.load("test");
    auto end = std::chrono::system_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "module `test` ran in " << time.count() << "ms" << std::endl;

    // Run the game loop
    data.services.running = true;
    while (data.services.running)
    {
        data.systems.Process(data);
    }
}

