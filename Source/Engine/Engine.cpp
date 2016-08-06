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
    state.load("test");

    // Run the game loop
    data.services.running = true;
    while (data.services.running)
    {
        data.systems.Process(data);
    }
}

