#include "LuaBinding.h"
#include <unordered_map>
#include <Common/Hash.h>

static HashMap<
    lua_State *,
    std::unordered_map<std::type_index, const LuaBinding *>
> REGISTERED_BINDINGS;

LuaBinding::LuaBinding(lua_State *L, HashMap<String, Callback> &&callbacks, PopSelf pop_self)
    : callbacks(callbacks), pop_self(pop_self)
{
    BuildMeta(L);
}

void LuaBinding::RegisterBinding(lua_State *L, std::type_index index, const LuaBinding *binding)
{
    REGISTERED_BINDINGS[L].emplace(index, binding);
}

std::optional<const LuaBinding*> LuaBinding::GetBinding(lua_State *L, std::type_index index)
{
    auto &bindings = REGISTERED_BINDINGS[L];
    auto it = bindings.find(index);
    if (it != bindings.end())
        return it->second;
    return std::nullopt;
}

std::optional<LuaBinding::Callback> LuaBinding::GetCallback(const String &name) const
{
    auto it = callbacks.find(name);
    if (it != callbacks.end())
        return it->second;
    return std::nullopt;
}

void LuaBinding::SetMetatable(lua_State *L) const
{
    metatable.push(L);
    lua_setmetatable(L, -2);
}

static int BindingMetaFunc(lua_State *L)
{
    auto callback = (LuaBinding::Callback)lua_touserdata(L, lua_upvalueindex(1));
    auto pop_self = (LuaBinding::PopSelf)lua_touserdata(L, lua_upvalueindex(2));

    try
    {
        return callback(pop_self(L), L);
    }
    catch (lua_arg_error e)
    {
        return luaL_typerror(L, e.narg, e.type->name());
    }
    catch (const std::exception &e)
    {
        lua_pushstring(L, e.what());
        return lua_error(L);
    }
}

void LuaBinding::BuildMeta(lua_State *L)
{
    lua_createtable(L, 0, (int)callbacks.size());
    for (auto &callback : callbacks)
    {
        auto name = callback.first.span();
        lua_pushlstring(L, name.data(), name.size());
        lua_pushlightuserdata(L, (void *)callback.second);
        lua_pushlightuserdata(L, (void *)pop_self);
        lua_pushcclosure(L, BindingMetaFunc, 2);
        lua_rawset(L, -3);
    }

    metatable = { L, -1 };
    lua_pop(L, 1);
}

