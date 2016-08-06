#include "pch.h"
#include "LuaEntitySystem.h"
#include "Entity.h"
#include "GameData.h"

LuaEntitySystem::LuaEntitySystem(int priority, String name, std::unique_ptr<EntityFilter> filter, const LuaValue &meta)
    : WatchSystem(priority, std::move(filter)), name_(std::move(name).into_owned())
{
    auto L = meta.state();
    lua_createtable(L, 0, 0);
    meta.push();
    lua_setmetatable(L, -2);
}

LuaEntitySystem::~LuaEntitySystem()
{
    // Remove the __sys field, so there's no accidental accesses
    auto L = lua_obj_.state();
    lua_obj_.push();
    lua_pushnil(L);
    lua_setfield(L, -2, "__sys");
    lua_pop(L, 1);
}

void LuaEntitySystem::Process(GameData &data)
{
    auto L = lua_obj_.state();
    lua_obj_.push();
    lua_getmetatable(L, -1);
    data.PushLua(L);
}

String LuaEntitySystem::GetName() const
{
    return String(name_.span());
}

struct LESIter
{
    LuaEntitySystem *system;
    std::optional<HashSet<Entity, Fnv1A>::const_iterator> iter;
};

static int iterStep(lua_State *L)
{
    auto iter = (LESIter *)lua_touserdata(L, 1);
    if (!iter->iter)
    {
        iter->iter = iter->system->begin();
    }
    else
    {
        ++iter->iter.value();
    }

    lua_pushnil(L);
    Entity::PushLua(L, **iter->iter);
    return 2;
}

static int iter(lua_State *L)
{
    auto system = (LuaEntitySystem *)lua_touserdata(L, 1);

    lua_pushcclosure(L, iterStep, 0);
    auto iter = (LESIter *)lua_newuserdata(L, sizeof(LESIter));
    iter->system = system;
    iter->iter = std::nullopt;
    lua_pushnil(L);

    return 3;
}

void LuaEntitySystem::InitializeLuaModule(lua_State *L)
{
    lua_createtable(L, 0, 1);

    lua_pushcclosure(L, iter, 0);
    lua_setfield(L, -2, "iter");
}
