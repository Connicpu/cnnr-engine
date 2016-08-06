#include "pch.h"
#include "Entity.h"
#include "GameData.h"
#include <Common/Hash.h>
#include <LuaInterface/LuaValue.h>

static HashMap<lua_State *, LuaValue> ENTITY_METATABLE;

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
    auto it = ENTITY_METATABLE.find(L);
    if (it != ENTITY_METATABLE.end())
    {
        it->second.push();
        return;
    }

    lua_createtable(L, 0, 0);

    lua_pushcclosure(L, entity_tostring, 0);
    lua_setfield(L, -2, "__tostring");

    ENTITY_METATABLE.insert_or_assign(L, LuaValue{ L, -1 });
}

void Entity::PushLua(lua_State *L, Entity e)
{
    *((Entity *)lua_newuserdata(L, sizeof(L))) = e;
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
