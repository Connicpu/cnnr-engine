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
    lua_getfield(L, LUA_GLOBALSINDEX, "require");
    "engine.gameData"_s.push_lua(L);
    lua_call(L, 1, 1);
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
    auto name = String::from_lua(L, 1);
    lua_getfield(L, LUA_GLOBALSINDEX, "__GAME_STATE");
    auto &data = *(GameData *)lua_touserdata(L, -1);

    if (auto id = data.components.FindList(name))
    {
        ComponentList *list = &data.components[*id];
        lua_pushlightuserdata(L, list);
        return 1;
    }

    return 0;
}

static int getComponent(lua_State *L)
{
    auto list = (ComponentList *)lua_touserdata(L, 1);
    auto ent = Entity::FromLua(L, 2);
    if (!ent) return luaL_typerror(L, 2, "Entity");

    if (auto component = list->get(*ent))
    {
        component->PushLuaBinding(L);
        return 1;
    }

    return 0;
}

static int getGraphics(lua_State *L)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "__GAME_STATE");
    auto &data = *(GameData *)lua_touserdata(L, -1);

    lua_pushlightuserdata(L, &*data.services.graphics);
    return 1;
}

static int getWindowSize(lua_State *L)
{
    auto graphics = (GraphicsService *)lua_touserdata(L, 1);
    lua_pushnumber(L, graphics->width);
    lua_pushnumber(L, graphics->height);
    return 1;
}

void GameData::InitializeLuaModule(lua_State *L)
{
    lua_createtable(L, 0, 3);

    lua_pushcclosure(L, getDelta, 0);
    lua_setfield(L, -2, "getDelta");

    lua_pushcclosure(L, getComponentList, 0);
    lua_setfield(L, -2, "getComponentList");

    lua_pushcclosure(L, getComponent, 0);
    lua_setfield(L, -2, "getComponent");

    lua_pushcclosure(L, getGraphics, 0);
    lua_setfield(L, -2, "getGraphics");

    lua_pushcclosure(L, getWindowSize, 0);
    lua_setfield(L, -2, "getWindowSize");
}
