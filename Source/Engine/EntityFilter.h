#pragma once

#include <memory>
#include <vector>
#include <Common/String.h>
#include "ComponentId.h"
#include "ComponentManager.h"

struct GameData;
class Entity;

class EntityFilter
{
public:
    virtual ~EntityFilter() {}

    // Move this value into a type-erased heap-allocated pointer
    virtual std::unique_ptr<EntityFilter> Erase() && = 0;
    // Copy this value into a type-erased heap-allocated pointer
    virtual std::unique_ptr<EntityFilter> Clone() const& = 0;
    // Combine this filter with another filter (moves *this)
    virtual std::unique_ptr<EntityFilter> Combine(std::unique_ptr<EntityFilter> other) &&;
    // Combine this filter with another filter (copies *this)
    virtual std::unique_ptr<EntityFilter> Combine(std::unique_ptr<EntityFilter> other) const&;

    // Determines whether this filter might match based only on
    // whether required components exist on the entity.
    virtual bool CanMatch(const GameData &data, Entity e) const = 0;
    // Determines if the entity completely matches the filter,
    // and can use data that might not raise an EntityEvent
    // when changed
    virtual bool IsMatch(const GameData &data, Entity e) const = 0;
};

class ComponentExistsFilter : public EntityFilter
{
public:
    ComponentExistsFilter(ComponentId id);

    virtual std::unique_ptr<EntityFilter> Erase() && override;
    virtual std::unique_ptr<EntityFilter> Clone() const& override;

    virtual bool CanMatch(const GameData &data, Entity e) const override;
    virtual bool IsMatch(const GameData &data, Entity e) const override;

private:
    ComponentId id_;
};

class MultiEntityFilter : public EntityFilter
{
public:
    MultiEntityFilter();
    MultiEntityFilter(std::vector<std::unique_ptr<EntityFilter>> list);
    MultiEntityFilter(const MultiEntityFilter &other);
    MultiEntityFilter(MultiEntityFilter &&) = default;

    virtual std::unique_ptr<EntityFilter> Erase() && override;
    virtual std::unique_ptr<EntityFilter> Clone() const& override;
    virtual std::unique_ptr<EntityFilter> Combine(std::unique_ptr<EntityFilter> other) && override;
    virtual std::unique_ptr<EntityFilter> Combine(std::unique_ptr<EntityFilter> other) const& override;

    virtual bool CanMatch(const GameData &data, Entity e) const override;
    virtual bool IsMatch(const GameData &data, Entity e) const override;

private:
    std::vector<std::unique_ptr<EntityFilter>> filters_;
};

template <typename ...Names>
std::unique_ptr<EntityFilter> component_list_filter(const ComponentManager &components, Names &&...names)
{
    std::unique_ptr<EntityFilter> afilters[sizeof...(Names)] =
    {
        ComponentExistsFilter(*components.FindList(names)).Erase()...
    };
    std::vector<std::unique_ptr<EntityFilter>> filters;
    filters.reserve(sizeof...(Names));
    for (auto &filter : afilters)
        filters.push_back(std::move(filter));
    return MultiEntityFilter(std::move(filters)).Erase();
}

