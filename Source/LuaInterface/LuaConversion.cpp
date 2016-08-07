#include "pch.h"
#include "LuaConversion.h"

template <>
String lua_object_cast<String>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TSTRING)
        throw std::bad_cast{};

    size_t len;
    const char *str = lua_tolstring(L, index, &len);
    return String{ gsl::cstring_span<>{ str, static_cast<ptrdiff_t>(len) } };
}

template <>
const char *lua_object_cast<const char *>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TSTRING)
        throw std::bad_cast{};

    return lua_tostring(L, index);
}

template <>
std::string lua_object_cast<std::string>(lua_State *L, int index)
{
    return std::move(lua_object_cast<String>(L, index).as_owned());
}

template <>
int8_t lua_object_cast<int8_t>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<int8_t>(lua_tonumber(L, index));
}

template <>
uint8_t lua_object_cast<uint8_t>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<uint8_t>(lua_tonumber(L, index));
}

template <>
int16_t lua_object_cast<int16_t>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<int16_t>(lua_tonumber(L, index));
}

template <>
uint16_t lua_object_cast<uint16_t>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<uint16_t>(lua_tonumber(L, index));
}

template <>
int32_t lua_object_cast<int32_t>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<int32_t>(lua_tonumber(L, index));
}

template <>
uint32_t lua_object_cast<uint32_t>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<uint32_t>(lua_tonumber(L, index));
}

template <>
int64_t lua_object_cast<int64_t>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<int64_t>(lua_tonumber(L, index));
}

template <>
uint64_t lua_object_cast<uint64_t>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<uint64_t>(lua_tonumber(L, index));
}

template <>
float lua_object_cast<float>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<float>(lua_tonumber(L, index));
}

template <>
double lua_object_cast<double>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<double>(lua_tonumber(L, index));
}

template <>
long double lua_object_cast<long double>(lua_State *L, int index)
{
    if (lua_type(L, index) != LUA_TNUMBER)
        throw std::bad_cast{};
    return static_cast<long double>(lua_tonumber(L, index));
}
