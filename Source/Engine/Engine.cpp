#include "pch.h"
#include "Engine.h"
#include "GameData.h"
#include "Transform.h"
#include "LuaModules.h"
#include "Perf.h"
#include <Common/LibLoading.h>
#include <Common/MathLib.h>
#include <Renderer/Renderer.h>
#include <LuaInterface/LuaState.h>
#include <LuaInterface/ModulePack.h>
#include <connorlib/serialization/toml.h>
#include <LuaInterface/RequireLoader.h>
#include <iomanip>
#include <random>

extern "C" void RunEngine(const EngineOptions &options)
{
    lua_cache_dir = fs::temp_directory_path() / "cnnr-engine" / "script-cache";
    fs::create_directories(*lua_cache_dir);

    // Create the lua state
    LuaState state;
    RegisterLuaModules(state);

    // Initialize the game data
    GameData data(state);

    // Open the graphics system
    data.services.graphics = GraphicsService(*options.backend);
    
    // Load lua components
    state.load("main");

    // Run the game loop
    data.services.running = true;
    while (data.services.running)
    {
        data.systems.Process(data);
    }
}

