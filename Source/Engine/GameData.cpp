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

static int getDelta(lua_State *L)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "__GAME_STATE");
    auto &data = *(GameData *)lua_touserdata(L, -1);
    lua_pushnumber(L, data.services.delta);
    return 1;
}

static int getComponentList(lua_State *L)
{
    lua_pushnil(L);
    return 1;
}

void GameData::InitializeLuaModule(lua_State *L)
{

}
