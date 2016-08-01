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

    static LuaState *from_raw(lua_State *L);

private:
    lua_State *L;
    std::vector<std::function<void()>> on_closes;
};
