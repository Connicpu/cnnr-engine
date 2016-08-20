#pragma once

#include <memory>
#include <vector>
#include <Common/String.h>
#include "ComponentId.h"
#include "ComponentManager.h"

struct GameData;
class Entity;

class EntityFilter;
using FilterPtr = std::unique_ptr<EntityFilter>;

class EntityFilter
{
public:
    virtual ~EntityFilter() {}

    // Move this value into a type-erased heap-allocated pointer
    virtual FilterPtr Erase() && = 0;
    // Copy this value into a type-erased heap-allocated pointer
    virtual FilterPtr Clone() const& = 0;
    // Combine this filter with another filter (moves *this)
    virtual FilterPtr Combine(FilterPtr other) &&;
    // Combine this filter with another filter (copies *this)
    virtual FilterPtr Combine(FilterPtr other) const&;

    // Determines whether this filter might match based only on
    // whether required components exist on the entity.
    virtual bool CanMatch(const GameData &data, Entity e) const = 0;
    // Determines if the entity completely matches the filter,
    // and can use data that might not raise an EntityEvent
    // when changed
    virtual bool IsMatch(const GameData &data, Entity e) const = 0;

    static void InitializeLuaModule(lua_State *L);
};

class ComponentExistsFilter : public EntityFilter
{
public:
    ComponentExistsFilter(ComponentId id);

    virtual FilterPtr Erase() && override;
    virtual FilterPtr Clone() const& override;

    virtual bool CanMatch(const GameData &data, Entity e) const override;
    virtual bool IsMatch(const GameData &data, Entity e) const override;

private:
    ComponentId id_;
};

class MultiEntityFilter : public EntityFilter
{
public:
    MultiEntityFilter();
    MultiEntityFilter(std::vector<FilterPtr> list);
    MultiEntityFilter(const MultiEntityFilter &other);
    MultiEntityFilter(MultiEntityFilter &&) = default;

    virtual FilterPtr Erase() && override;
    virtual FilterPtr Clone() const& override;
    virtual FilterPtr Combine(FilterPtr other) && override;
    virtual FilterPtr Combine(FilterPtr other) const& override;

    virtual bool CanMatch(const GameData &data, Entity e) const override;
    virtual bool IsMatch(const GameData &data, Entity e) const override;

private:
    std::vector<FilterPtr> filters_;
};

template <typename ...Names>
FilterPtr component_list_filter(const ComponentManager &components, Names &&...names)
{
    FilterPtr afilters[sizeof...(Names)] =
    {
        ComponentExistsFilter(*components.FindList(names)).Erase()...
    };
    std::vector<FilterPtr> filters;
    filters.reserve(sizeof...(Names));
    for (auto &filter : afilters)
        filters.push_back(std::move(filter));
    return MultiEntityFilter(std::move(filters)).Erase();
}

