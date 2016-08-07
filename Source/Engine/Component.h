#pragma once

#include <Common/String.h>
#include <LuaInterface/LuaValue.h>

class Component
{
public:
    virtual ~Component() {}
    virtual String GetName() const;
    virtual void PushLuaBinding(lua_State *L);
    static Component *GetLuaBinding(lua_State *L, int idx, const std::type_info *&type);

protected:
    template <typename T, typename Func>
    static void SetupMetatable(lua_State *L, Func &&build);
};

template <typename T>
class component_name
{
public:
    static const char *const value;
};

template <typename T>
const char *const component_name<T>::value = typeid(T).name();

#define COMPONENT_NAME(T, name) \
template <> const char *const component_name<T>::value = (name)

template<typename T, typename Func>
inline void Component::SetupMetatable(lua_State *L, Func &&build)
{
    // Get the component metatables register, or create it if it doesn't exist
    lua_getfield(L, LUA_GLOBALSINDEX, "__COMPONENT_METATABLES");
    if (lua_type(L, -1) == LUA_TNIL)
    {
        lua_pop(L, 1);
        lua_createtable(L, 0, 1);
        lua_pushvalue(L, -1);
        lua_setfield(L, LUA_GLOBALSINDEX, "__COMPONENT_METATABLES");
    }

    // Get the metatable if it exists, and create it if it doesn't
    lua_getfield(L, -1, typeid(T).name());
    if (lua_type(L, -1) == LUA_TTABLE)
    {
        lua_remove(L, -2);
        return;
    }

    lua_pop(L, 1);
    lua_createtable(L, 0, 0);

    build();

    lua_pushvalue(L, -1);
    lua_setfield(L, -3, typeid(T).name());
    lua_remove(L, -2);
    return;
}
