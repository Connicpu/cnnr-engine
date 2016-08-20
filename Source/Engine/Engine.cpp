#include "pch.h"
#include "Engine.h"
#include "GameData.h"
#include "Transform.h"
#include "LuaModules.h"
#include "Perf.h"
#include "Sprite.h"
#include "LuaEntitySystem.h"
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

    // Open the sprite loader
    data.services.sprite_loader = SpriteLoader(data.services.graphics->GetDevice());
    
    // Load lua components
    state.load("main");

    // Load my test system
    auto L = state.state();
    lua_getfield(L, LUA_GLOBALSINDEX, "require");
    "systems.misc.spin"_s.push_lua(L);
    lua_call(L, 1, 1);
    LuaValue spin{ L, -1 };
    lua_pop(L, 1);
    data.systems.RegisterSystem(LuaEntitySystem::Build(spin));

    // Create some test data
    {
        using namespace Math;
        auto device = data.services.graphics->GetDevice();
        auto &main_scene = data.services.scenes["main"_s];
        data.services.scene_order = { "main"_s };
        device->CreateScene(&main_scene);
        device->CreateCamera(&data.services.camera);

        auto test_pack = data.services.sprite_loader->Load("Test");
        auto e = data.entities.CreateEntity();

        data.components.transform.insert(e);

        data.components.sprite.insert(e);
        SpriteObjectParams params;
        params.is_static = false;
        params.is_translucent = false;
        params.layer = 0.0f;
        params.texture = test_pack->GetSprite(0);
        params.tint = Color(1, 1, 1);
        params.transform = Matrix3x2::Identity();
        params.window = *test_pack->GetSpriteWindow(0);
        data.components.sprite[e].Initialize(main_scene.p, test_pack, params);

        data.entities.FlushQueue(data);
    }

    // Run the game loop
    data.services.running = true;
    while (data.services.running)
    {
        data.systems.Process(data);
    }

    state.load("onexit");
}

