#pragma once

#include "IncludeLua.h"
#include <Common/optional.h>

class LuaValue
{
public:
    LuaValue();
    LuaValue(const LuaValue &other);
    LuaValue(LuaValue &&other);
    LuaValue(lua_State *L, int stack_index);
    ~LuaValue();

    LuaValue &operator=(const LuaValue &other);
    LuaValue &operator=(LuaValue &&other);

    void push(lua_State *L) const;
    void push() const;
    lua_State *state() const;

    void reset();

private:
    lua_State *lua_state_;
    int handle_;
};

