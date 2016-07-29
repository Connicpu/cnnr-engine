#pragma once

#include "LuaValue.h"
#include <Common/optional.h>

class LuaBinding;

class LuaObject
{
public:
    struct UserdataRepr
    {
        static const uint32_t PATTERN = 0xac3eb244;
        uint32_t pattern;
        bool freed;
        LuaObject *obj;
        const LuaBinding *binding;
    };

    LuaObject(const LuaObject &) = delete;
    LuaObject(LuaObject &&move);

    LuaObject &operator=(const LuaObject &) = delete;
    LuaObject &operator=(LuaObject &&move);

    virtual ~LuaObject();
    virtual const LuaBinding *GetBinding(lua_State *L) = 0;

    void Push(lua_State *L);

private:
    std::optional<LuaValue> userdata_;
};
