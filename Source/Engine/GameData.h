#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "ServiceManager.h"
#include <LuaInterface/LuaState.h>

struct GameData
{
    GameData(LuaState &state);

    EntityManager entities;
    ComponentManager components;
    SystemManager systems;
    ServiceManager services;

    void PushLua(lua_State *L);

private:
    LuaValue lua_obj_;
};
