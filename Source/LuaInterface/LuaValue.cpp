#include "pch.h"
#include "LuaValue.h"

LuaValue::LuaValue()
    : lua_state_(nullptr), handle_(LUA_NOREF)
{
}

LuaValue::LuaValue(const LuaValue &other)
    : lua_state_(other.lua_state_)
{
    if (lua_state_)
    {
        lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, handle_);
        handle_ = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
    }
}

LuaValue::LuaValue(LuaValue &&other)
    : LuaValue()
{
    std::swap(*this, other);
}

LuaValue::LuaValue(lua_State *L, int stack_index)
    : lua_state_(L)
{
    lua_pushvalue(L, stack_index);
    handle_ = luaL_ref(L, LUA_REGISTRYINDEX);
}

LuaValue::~LuaValue()
{
    reset();
}

LuaValue &LuaValue::operator=(const LuaValue &other)
{
    reset();
    lua_state_ = other.lua_state_;
    if (lua_state_)
    {
        lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, handle_);
        handle_ = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
    }

    return *this;
}

LuaValue &LuaValue::operator=(LuaValue &&other)
{
    reset();
    std::swap(*this, other);
    return *this;
}

void LuaValue::push(lua_State *L) const
{
    if (lua_state_)
    {
        assert(L == lua_state_ && "LuaValue attempted to be pushed into lua_State that it doesn't belong to");
    }

    if (handle_ == LUA_REFNIL || handle_ == LUA_NOREF)
    {
        lua_pushnil(L);
    }
    else
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, handle_);
    }
}

void LuaValue::push() const
{
    assert(lua_state_);
    push(lua_state_);
}

lua_State *LuaValue::state() const
{
    return lua_state_;
}

void LuaValue::reset()
{
    if (lua_state_ && handle_ != LUA_NOREF)
    {
        luaL_unref(lua_state_, LUA_REGISTRYINDEX, handle_);
    }

    lua_state_ = nullptr;
    handle_ = LUA_NOREF;
}
