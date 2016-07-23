#pragma once

#include "IncludeLua.h"
#include <Common/optional.h>

class LuaValue;

namespace std
{
    template <>
    void swap<LuaValue>(LuaValue &lhs, LuaValue &rhs);
}

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
    friend void std::swap(LuaValue &lhs, LuaValue &rhs);

    lua_State *lua_state_;
    int handle_;
};

