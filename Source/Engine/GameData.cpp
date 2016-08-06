#include "pch.h"
#include "GameData.h"

GameData::GameData(LuaState &state)
    : entities(), components(&entities), systems(components), services()
{
    lua_pushlightuserdata(state.state(), this);
    lua_setfield(state.state(), LUA_GLOBALSINDEX, "__GAME_STATE");
}

void GameData::PushLua(lua_State *L)
{
    lua_newtable(L);
}
