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
