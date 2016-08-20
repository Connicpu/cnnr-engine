#include "pch.h"
#include "LuaModules.h"
#include "LuaEntitySystem.h"
#include "EntityFilter.h"
#include "GameData.h"

void RegisterLuaModules(LuaState &state)
{
    state.register_module<LuaEntitySystem>("__internal.entitySystem");
    state.register_module<EntityFilter>("__internal.filter");
    state.register_module<GameData>("__internal.gameData");
}
