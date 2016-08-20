#include "pch.h"
#include "EntityFilter.h"
#include "Entity.h"
#include "GameData.h"

FilterPtr EntityFilter::Combine(FilterPtr other) &&
{
    std::vector<FilterPtr> filters;
    filters.reserve(2);
    filters.push_back(std::move(*this).Erase());
    filters.push_back(std::move(other));
    return MultiEntityFilter(std::move(filters)).Erase();
}

FilterPtr EntityFilter::Combine(FilterPtr other) const &
{
    std::vector<FilterPtr> filters;
    filters.reserve(2);
    filters.push_back(Clone());
    filters.push_back(std::move(other));
    return MultiEntityFilter(std::move(filters)).Erase();
}

void EntityFilter::InitializeLuaModule(lua_State *L)
{
    lua_createtable(L, 0, 1);

    #pragma region metatable
    lua_createtable(L, 0, 2);
    lua_pushcclosure(L, [](lua_State *L) -> int
    {
        auto &lhs = *(const FilterPtr *)lua_touserdata(L, 1);
        auto &rhs = *(const FilterPtr *)lua_touserdata(L, 2);
        auto filter = lhs->Combine(rhs->Clone());
        new (lua_newuserdata(L, sizeof(FilterPtr))) FilterPtr(std::move(filter));
        return 1;
    }, 0);
    lua_setfield(L, -2, "__add");

    lua_pushcclosure(L, [](lua_State *L) -> int
    {
        ((FilterPtr *)lua_touserdata(L, 1))->~unique_ptr();
        return 1;
    }, 0);
    lua_setfield(L, -2, "__gc");

    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    #pragma endregion

    lua_pushcclosure(L, [](lua_State *L) -> int
    {
        auto name = String::from_lua(L, 1);
        lua_getfield(L, LUA_GLOBALSINDEX, "__GAME_STATE");
        auto &data = *(GameData *)lua_touserdata(L, -1);
        lua_pop(L, 1);

        if (auto id = data.components.FindList(name))
        {
            auto filter = ComponentExistsFilter(*id).Erase();
            new (lua_newuserdata(L, sizeof(FilterPtr))) FilterPtr(std::move(filter));
            lua_pushvalue(L, lua_upvalueindex(1));
            lua_setmetatable(L, -2);
            return 1;
        }

        return luaL_error(L, "Failed to find a component named %s", lua_tostring(L, 1));
    }, 1);
    lua_setfield(L, -2, "component");
}

ComponentExistsFilter::ComponentExistsFilter(ComponentId id)
    : id_(id)
{
}

FilterPtr ComponentExistsFilter::Erase() &&
{
    return FilterPtr(new ComponentExistsFilter(std::move(*this)));
}

FilterPtr ComponentExistsFilter::Clone() const&
{
    return ComponentExistsFilter(*this).Erase();
}

bool ComponentExistsFilter::CanMatch(const GameData &data, Entity e) const
{
    return IsMatch(data, e);
}

bool ComponentExistsFilter::IsMatch(const GameData &data, Entity e) const
{
    return !!data.components[id_].get(e);
}

MultiEntityFilter::MultiEntityFilter()
{
}

MultiEntityFilter::MultiEntityFilter(std::vector<FilterPtr> list)
    : filters_(std::move(list))
{
}

MultiEntityFilter::MultiEntityFilter(const MultiEntityFilter &other)
{
    filters_.reserve(other.filters_.size());
    for (auto &filter : other.filters_)
    {
        filters_.push_back(filter->Clone());
    }
}

FilterPtr MultiEntityFilter::Erase() &&
{
    return FilterPtr(new MultiEntityFilter(std::move(*this)));
}

FilterPtr MultiEntityFilter::Clone() const&
{
    return MultiEntityFilter(*this).Erase();
}

FilterPtr MultiEntityFilter::Combine(FilterPtr other) &&
{
    filters_.push_back(std::move(other));
    return std::move(*this).Erase();
}

FilterPtr MultiEntityFilter::Combine(FilterPtr other) const&
{
    return MultiEntityFilter(*this).Combine(std::move(other));
}

bool MultiEntityFilter::CanMatch(const GameData &data, Entity e) const
{
    for (auto &filter : filters_)
    {
        if (!filter->CanMatch(data, e))
            return false;
    }
    return true;
}

bool MultiEntityFilter::IsMatch(const GameData &data, Entity e) const
{
    for (auto &filter : filters_)
    {
        if (!filter->IsMatch(data, e))
            return false;
    }
    return true;
}
