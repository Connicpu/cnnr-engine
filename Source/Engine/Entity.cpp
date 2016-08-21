#include "pch.h"
#include "Entity.h"
#include "GameData.h"
#include <Common/Hash.h>
#include <LuaInterface/LuaValue.h>

static int entity_tostring(lua_State *L)
{
    auto e = *Entity::FromLua(L, 1);
    lua_getfield(L, LUA_GLOBALSINDEX, "__GAME_STATE");
    auto data = (GameData *)lua_touserdata(L, -1);
    data->entities.GetName(e).push_lua(L);
    return 1;
}

static void GetEntityMetatable(lua_State *L)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "__ENTITY_METATABLE");
    if (lua_type(L, -1) != LUA_TNIL)
    {
        return;
    }
    lua_pop(L, 1);

    lua_createtable(L, 0, 0);

    lua_pushcclosure(L, entity_tostring, 0);
    lua_setfield(L, -2, "__tostring");

    lua_pushvalue(L, -1);
    lua_setfield(L, LUA_GLOBALSINDEX, "__ENTITY_METATABLE");
}

void Entity::PushLua(lua_State *L, Entity e)
{
    new (lua_newuserdata(L, sizeof(Entity))) Entity(e);
    GetEntityMetatable(L);
    lua_setmetatable(L, -2);
}

std::optional<Entity> Entity::FromLua(lua_State *L, int idx)
{
    void *e = lua_touserdata(L, idx);

    lua_getmetatable(L, idx);
    GetEntityMetatable(L);

    if (!lua_rawequal(L, -1, -2))
        return std::nullopt;

    lua_pop(L, 2);

    return *(Entity *)e;
}
