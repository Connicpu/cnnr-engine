#include "pch.h"
#include "LuaModules.h"
#include "LuaEntitySystem.h"

void RegisterLuaModules(LuaState &state)
{
    state.register_module<LuaEntitySystem>("__internal.entitySystem");
}
