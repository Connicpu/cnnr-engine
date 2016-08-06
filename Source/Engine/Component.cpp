#include "pch.h"
#include "Component.h"
#include <Common/Hash.h>
#include <typeindex>
#include <typeinfo>

static HashMap<lua_State *, std::unordered_map<std::type_index, LuaValue>> NO_BINDING;

String Component::GetName() const
{
    return "Unnamed component"_s;
}

static void PushMetatable(lua_State *L, const std::type_info &id, const String &cls)
{
    auto lit = NO_BINDING.find(L);
    if (lit != NO_BINDING.end())
    {
        auto it = lit->second.find(id);
        if (it != lit->second.end())
        {
            it->second.push();
            return;
        }
    }

    lua_assertstack(L, 3);

    // Create a metatable for the binding
    lua_createtable(L, 0, 5);
    auto table = lua_gettop(L);

    // ["__metatable"] = nil to prevent snooping on this object
    lua_pushstring(L, "__metatable");
    lua_pushnil(L);
    lua_rawset(L, table);

    // Add __name to the metatable (just a convention for classes)
    lua_pushstring(L, "__name");
    cls.push_lua(L);
    lua_rawset(L, table);

    // Give the raw cpp class as well, marking it
    lua_pushstring(L, "__cpp_type_name");
    lua_pushstring(L, id.name());
    lua_rawset(L, table);

    // Insert the raw typeinfo as well
    lua_pushstring(L, "__cpp_type_info");
    lua_pushlightuserdata(L, (void *)&id);
    lua_rawset(L, table);

    // Add a tostring impl
    lua_pushstring(L, "__tostring");
    lua_pushcclosure(L, [](lua_State *L)
    {
        lua_pushstring(L, "__name");
        lua_rawget(L, -2);
        auto name = String::from_lua(L, -1);
        auto desc = "Component["_s + name + "]"_s;
        desc.push_lua(L);
        return 1;
    }, 0);
    lua_rawset(L, table);

    LuaValue binding{ L, -1 };
    NO_BINDING[L].insert_or_assign(id, std::move(binding));

    assert(lua_gettop(L) == table);
}

void Component::PushLuaBinding(lua_State *L)
{
    lua_assertstack(L, 3);

    // Create a table for the object
    lua_createtable(L, 0, 0);
    auto table = lua_gettop(L);

    // Push in the lightuserdata
    lua_pushstring(L, "__cpp_ptr");
    lua_pushlightuserdata(L, this);
    lua_rawset(L, table);

    // Set the metatable
    PushMetatable(L, typeid(*this), GetName());
    lua_setmetatable(L, table);

    assert(lua_gettop(L) == table);
}

Component *Component::GetLuaBinding(lua_State *L, int idx, const std::type_info *&type)
{
    lua_assertstack(L, 2);

    if (lua_type(L, idx) != LUA_TTABLE)
    {
        type = nullptr;
        return nullptr;
    }

    if (!lua_getmetatable(L, idx))
    {
        type = nullptr;
        return nullptr;
    }

    lua_pushstring(L, "__cpp_type_info");
    lua_rawget(L, -2);
    auto type_info = (const std::type_info *)lua_touserdata(L, -1);
    lua_pop(L, 2); // pop type_info, metatable

    if (*type_info != *type)
    {
        type = type_info;
        return nullptr;
    }

    lua_pushvalue(L, idx);
    lua_pushstring(L, "__cpp_ptr");
    lua_rawget(L, -2);
    auto ptr = lua_touserdata(L, -1);
    lua_pop(L, 2); // pop ptr, extra pushvalue

    return (Component *)ptr;
}
