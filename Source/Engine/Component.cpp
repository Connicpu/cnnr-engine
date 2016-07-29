#include "Component.h"
#include <Common/Hash.h>
#include <typeindex>

static HashMap<lua_State *, std::unordered_map<std::type_index, LuaValue>> NO_BINDING;

String Component::GetName()
{
    return "Unnamed component"_s;
}

void Component::PushLuaBinding(lua_State *L)
{
    auto lit = NO_BINDING.find(L);
    if (lit != NO_BINDING.end())
    {
        auto it = lit->second.find(typeid(*this));
        if (it != lit->second.end())
        {
            it->second.push();
            return;
        }
    }

    // Create an object to use for the empty binding
    lua_createtable(L, 0, 0);
    // Create a metatable for the binding
    lua_createtable(L, 0, 3);

    // Add __name to the metatable (just a convention for classes)
    lua_pushstring(L, "__name");
    GetName().push_lua(L);
    lua_rawset(L, -3);

    // Add a tostring impl
    lua_pushstring(L, "__tostring");
    lua_pushcclosure(L, [](lua_State *L)
    {
        lua_pushstring(L, "__name");
        lua_rawget(L, -2);
        auto name = String::from_lua(L, -1);
        auto desc = "Component["_s + name + "] (no lua bindings)"_s;
        lua_pop(L, 2);
        desc.push_lua(L);
        return 1;
    }, 0);
    lua_rawset(L, -3);

    // Add the metatable
    lua_setmetatable(L, -2);
    
    LuaValue binding{ L, -1 };
    NO_BINDING[L].insert_or_assign(typeid(*this), std::move(binding));
}
