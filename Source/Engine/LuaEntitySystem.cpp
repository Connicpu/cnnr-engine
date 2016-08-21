#include "pch.h"
#include "LuaEntitySystem.h"
#include "Entity.h"
#include "GameData.h"

LuaEntitySystem::LuaEntitySystem(int priority, String name, std::unique_ptr<EntityFilter> filter, LuaValue &&table)
    : WatchSystem(priority, std::move(filter)), name_(std::move(name).into_owned()), lua_obj_(std::move(table))
{
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

    lua_getfield(L, -1, "process");
    lua_pushvalue(L, -2);
    data.PushLua(L);
    if (lua_pcall(L, 2, 0, 0))
    {
        auto msg = String::from_lua(L, -1).into_stdstring();
        throw std::runtime_error{ msg };
    }

    lua_pop(L, 1);
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

static int iterReset(lua_State *L)
{
    auto system = (LuaEntitySystem *)lua_touserdata(L, 1);
    system->get_iter() = std::nullopt;
    return 0;
}

static int iterStep(lua_State *L)
{
    auto system = (LuaEntitySystem *)lua_touserdata(L, 1);
    auto &iter = system->get_iter();

    if (!iter)
        iter = system->begin();
    else
        ++*iter;

    lua_getfield(L, LUA_GLOBALSINDEX, "__GAME_STATE");
    auto &data = *(GameData *)lua_touserdata(L, -1);
    lua_pop(L, 1);
    while (*iter != system->end() && !system->IsMatch(data, **iter))
    {
        ++*iter;
    }

    lua_pushboolean(L, *iter != system->end());
    return 1;
}

static int iterValue(lua_State *L)
{
    auto system = (LuaEntitySystem *)lua_touserdata(L, 1);
    auto &iter = system->get_iter();
    if (!iter || *iter == system->end())
        return luaL_error(L, "Invalid iterator dereference");

    Entity::PushLua(L, **iter);
    return 1;
}

void LuaEntitySystem::InitializeLuaModule(lua_State *L)
{
    lua_createtable(L, 0, 3);

    lua_pushcclosure(L, iterReset, 0);
    lua_setfield(L, -2, "iterReset");
    lua_pushcclosure(L, iterStep, 0);
    lua_setfield(L, -2, "iterStep");
    lua_pushcclosure(L, iterValue, 0);
    lua_setfield(L, -2, "iterValue");
}

bool LuaEntitySystem::IsMatch(const GameData &data, Entity e)
{
    return filter_->IsMatch(data, e);
}

SystemPtr LuaEntitySystem::Build(const LuaValue &meta)
{
    auto L = meta.state();
    lua_createtable(L, 0, 2);
    auto obj = lua_gettop(L);

    meta.push();
    lua_setmetatable(L, obj);

    auto sysPtr = (LuaEntitySystem *)::operator new(sizeof(LuaEntitySystem));

    lua_getfield(L, obj, "__initialize");
    if (lua_type(L, -1) != LUA_TFUNCTION)
        throw std::runtime_error{ "A system's `__initialize` got overridden!" };

    lua_pushvalue(L, obj);
    lua_pushlightuserdata(L, sysPtr);
    if (lua_pcall(L, 2, 0, 0))
    {
        auto msg = String::from_lua(L, -1).into_stdstring();
        throw std::runtime_error{ msg };
    }

    lua_getfield(L, obj, "meta");
    lua_getfield(L, -1, "name");
    auto sysName = String::from_lua(L, -1).into_owned();
    lua_pop(L, 1);
    lua_getfield(L, -1, "priority");
    auto sysPrio = (int)lua_tonumber(L, -1);
    lua_pop(L, 2);

    lua_getfield(L, obj, "entityFilter");
    auto filter = std::move(*(FilterPtr *)lua_touserdata(L, -1));
    lua_pop(L, 1);

    new (sysPtr) LuaEntitySystem(sysPrio, std::move(sysName), std::move(filter), LuaValue(L, obj));

    lua_pop(L, 1);
    return SystemPtr(sysPtr);
}

std::optional<WatchSystem::iterator> &LuaEntitySystem::get_iter()
{
    return iter_;
}
