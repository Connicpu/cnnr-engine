#pragma once

#include <Common/String.h>
#include "IncludeLua.h"
#include <vector>
#include <functional>

class LuaState
{
public:
    LuaState();
    ~LuaState();

    LuaState(const LuaState &) = delete;
    LuaState &operator=(const LuaState &) = delete;

    lua_State *state();
    void load(const String &file);

    template <typename T>
    void register_module(const String &name);

    static LuaState *from_raw(lua_State *L);

private:
    lua_State *L;
    std::vector<std::function<void()>> on_closes;
};

template <typename T>
inline void LuaState::register_module(const String &name)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "package");
    lua_getfield(L, -1, "loaded");
    name.push_lua(L);
    lua_gettable(L, -2);
    if (lua_type(L, -1) != LUA_TNIL)
    {
        lua_pop(L, 3);
        return;
    }
    else
    {
        lua_pop(L, 1);
    }

    name.push_lua(L);
    int top = lua_gettop(L);
    T::InitializeLuaModule(L);
    assert((lua_gettop(L) == top + 1) && "T::InitializeLuaModule is an unbalanced function!");
    
    lua_settable(L, -3);
}
