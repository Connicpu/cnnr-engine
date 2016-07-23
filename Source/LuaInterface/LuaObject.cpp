#include "LuaObject.h"
#include "LuaBinding.h"

LuaObject::~LuaObject()
{
    if (userdata_)
    {
        auto L = userdata_->state();
        userdata_->push(L);

        auto data = (UserdataRepr *)lua_touserdata(L, -1);
        data->freed = true;

        lua_pop(L, 1);
    }
}

void LuaObject::Push(lua_State *L)
{
    if (userdata_)
    {
        userdata_->push(L);
        return;
    }

    auto data = (UserdataRepr *)lua_newuserdata(L, sizeof(UserdataRepr));
    data->pattern = UserdataRepr::PATTERN;
    data->freed = false;
    data->obj = this;
    data->binding = GetBinding(L);
    data->binding->SetMetatable(L);
    userdata_ = LuaValue{ L, -1 };
    // The userdata is at the top of the stack now
}

