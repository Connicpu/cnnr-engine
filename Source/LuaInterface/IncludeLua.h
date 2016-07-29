#pragma once

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#include <cassert>

inline int lua_assertstack(lua_State *L, int sz)
{
    int result = lua_checkstack(L, sz);
    assert(result);
    return result;
}
