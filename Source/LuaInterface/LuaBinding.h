#pragma once

#include <typeinfo>
#include <typeindex>
#include <Common/optional.h>
#include <Common/Hash.h>
#include <Common/String.h>
#include "LuaObject.h"
#include "LuaConversion.h"

template <typename T>
class LuaBindingBuilder;

class LuaBinding
{
public:
    using Callback = int(*)(void *self, lua_State *L);
    using PopSelf = void *(*)(lua_State *L);

    template <typename T>
    static const LuaBinding *Build(lua_State *L, const LuaBindingBuilder<T> &&builder);
    static void RegisterBinding(lua_State *L, std::type_index index, const LuaBinding *binding);
    static std::optional<const LuaBinding *> GetBinding(lua_State *L, std::type_index index);

    std::optional<Callback> GetCallback(const String &name) const;
    void SetMetatable(lua_State *L) const;

private:
    LuaBinding(lua_State *L, HashMap<String, Callback> &&callbacks, PopSelf pop_self);
    void BuildMeta(lua_State *L);

    HashMap<String, Callback> callbacks;
    PopSelf pop_self;
    LuaValue metatable;
};

template <typename T>
class LuaBindingBuilder
{
public:
    using Callback = int(*)(T &self, lua_State *L);

    void Add(const String &name, Callback cb);

private:
    friend class LuaBinding;

    HashMap<String, LuaBinding::Callback> callbacks;
};

template <typename Self>
class LuaObjectHelper : public virtual LuaObject
{
public:
    static void InitializeBindings(LuaBindingBuilder<Self> &binding);
    virtual const LuaBinding *GetBinding(lua_State *L) override;
};

template<typename T>
inline const LuaBinding *LuaBinding::Build(lua_State *L, const LuaBindingBuilder<T> &&builder)
{
    return new LuaBinding(L, std::move(builder.callbacks), [](lua_State *L)
    {
        if (auto self = lua_object_cast<std::optional<T &>>(L, 1))
            return &*self;

        luaL_typerror(L, 1, typeid(T).name());
    });
}

template<typename Self>
inline void LuaObjectHelper<Self>::InitializeBindings(LuaBindingBuilder<Self> &)
{
}

template<typename Self>
inline const LuaBinding *LuaObjectHelper<Self>::GetBinding(lua_State *L)
{
    if (auto binding = LuaBinding::GetBinding(L, typeid(Self)))
        return *binding;

    LuaBindingBuilder<Self> builder;
    Self::InitializeBindings(builder);
    auto *binding = LuaBinding::Build<Self>(L, builder);
    LuaBinding::RegisterBinding(L, typeid(Self), binding);

    return binding;
}

template<typename T>
inline void LuaBindingBuilder<T>::Add(const String &name, Callback cb)
{
    callbacks.emplace(name, (LuaBinding::Callback)cb);
}
