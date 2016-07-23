#pragma once

#include <typeinfo>
#include <Common/String.h>
#include "IncludeLua.h"
#include "LuaObject.h"

struct lua_arg_error : public std::exception
{
public:
    lua_arg_error(int narg, const type_info *type)
        : narg(narg), type(type)
    {
    }

    int narg;
    const type_info *type;
};

struct lua_freed_error : public std::exception
{
    virtual const char *what() const noexcept override
    {
        return "Tried to access a bound C++ value that was already freed";
    }
};

namespace details
{

    template <typename T>
    struct LuaObjConverter
    {
        static T convert(lua_State *L, int index)
        {
            static_assert(false,
                "There is no way to perform the conversion you are asking."
                "Add an overload, or use a reference if your object inherits LuaObject.");
        }
    };

    template <typename T>
    struct LuaObjConverter<T &>
    {
        static T &convert(lua_State *L, int index)
        {
            static_assert(
                std::is_base_of<LuaObject, T>::value,
                "You must create an explicit overload of lua_object_cast"
                "for items that don't inherit LuaObject");

            if (lua_type(L, index) != LUA_TUSERDATA)
                throw lua_arg_error(index, typeid(T)); // Only a userdata can possibly be what we're looking for
            if (lua_objlen(L, index) != sizeof(LuaObject::UserdataRepr))
                throw lua_arg_error(index, typeid(T)); // If it's the wrong size then it's right out
            auto data = (LuaObject::UserdataRepr *)lua_touserdata(L, index);
            if (data->pattern != LuaObject::UserdataRepr::PATTERN)
                throw lua_arg_error(index, typeid(T)); // The pattern didn't match- oops
            if (data->freed)
                throw lua_freed_error();
            return dynamic_cast<T &>(*data->obj);
        }
    };

    template <typename T>
    struct LuaObjConverter<const T &>
    {
        static const T &convert(lua_State *L, int index)
        {
            return LuaObjConverter<T &>::convert(L, index);
        }
    };

    template <typename T>
    struct LuaObjConverter<std::optional<T>>
    {
        static std::optional<T> convert(lua_State *L, int index)
        {
            try
            {
                return LuaObjConverter<T>::convert(L, index);
            }
            catch (const lua_arg_error &)
            {
                return std::nullopt;
            }
        }
    };

    template <typename T>
    struct LuaObjConverter<T *>
    {
        static const T &convert(lua_State *L, int index)
        {
            if (std::optional<T &> val = LuaObjConverter<std::optional<T &>>::convert(L, index))
                return &*val;
        }
    };

    template <typename T>
    struct LuaObjConverter<const T *>
    {
        static const T &convert(lua_State *L, int index)
        {
            if (std::optional<const T &> val = LuaObjConverter<std::optional<const T &>>::convert(L, index))
                return &*val;
        }
    };
}

template <typename T>
T lua_object_cast(lua_State *L, int index)
{
    return details::LuaObjConverter<T>::convert(L, index);
}

template <>
String lua_object_cast<String>(lua_State *L, int index);
template <>
const char *lua_object_cast<const char *>(lua_State *L, int index);
template <>
std::string lua_object_cast<std::string>(lua_State *L, int index);

template <>
int8_t lua_object_cast<int8_t>(lua_State *L, int index);
template <>
uint8_t lua_object_cast<uint8_t>(lua_State *L, int index);
template <>
int16_t lua_object_cast<int16_t>(lua_State *L, int index);
template <>
uint16_t lua_object_cast<uint16_t>(lua_State *L, int index);
template <>
int32_t lua_object_cast<int32_t>(lua_State *L, int index);
template <>
uint32_t lua_object_cast<uint32_t>(lua_State *L, int index);
template <>
int64_t lua_object_cast<int64_t>(lua_State *L, int index);
template <>
uint64_t lua_object_cast<uint64_t>(lua_State *L, int index);
template <>
float lua_object_cast<float>(lua_State *L, int index);
template <>
double lua_object_cast<double>(lua_State *L, int index);
template <>
long double lua_object_cast<long double>(lua_State *L, int index);

