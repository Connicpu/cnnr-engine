#pragma once

#include <Common/String.h>
#include <LuaInterface/LuaValue.h>

class Component
{
public:
    virtual ~Component() {}
    virtual String GetName();
    virtual void PushLuaBinding(lua_State *L);
    static Component *GetLuaBinding(lua_State *L, int idx, const std::type_info *&type);
};
