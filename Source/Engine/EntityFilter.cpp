#include "pch.h"
#include "EntityFilter.h"
#include "Entity.h"
#include "GameData.h"

std::unique_ptr<EntityFilter> EntityFilter::Combine(std::unique_ptr<EntityFilter> other) &&
{
    std::vector<std::unique_ptr<EntityFilter>> filters;
    filters.reserve(2);
    filters.push_back(std::move(*this).Erase());
    filters.push_back(std::move(other));
    return MultiEntityFilter(std::move(filters)).Erase();
}

std::unique_ptr<EntityFilter> EntityFilter::Combine(std::unique_ptr<EntityFilter> other) const &
{
    std::vector<std::unique_ptr<EntityFilter>> filters;
    filters.reserve(2);
    filters.push_back(Clone());
    filters.push_back(std::move(other));
    return MultiEntityFilter(std::move(filters)).Erase();
}

ComponentExistsFilter::ComponentExistsFilter(ComponentId id)
    : id_(id)
{
}

std::unique_ptr<EntityFilter> ComponentExistsFilter::Erase() &&
{
    return std::unique_ptr<EntityFilter>(new ComponentExistsFilter(std::move(*this)));
}

std::unique_ptr<EntityFilter> ComponentExistsFilter::Clone() const&
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

MultiEntityFilter::MultiEntityFilter(std::vector<std::unique_ptr<EntityFilter>> list)
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

std::unique_ptr<EntityFilter> MultiEntityFilter::Erase() &&
{
    return std::unique_ptr<EntityFilter>(new MultiEntityFilter(std::move(*this)));
}

std::unique_ptr<EntityFilter> MultiEntityFilter::Clone() const&
{
    return MultiEntityFilter(*this).Erase();
}

std::unique_ptr<EntityFilter> MultiEntityFilter::Combine(std::unique_ptr<EntityFilter> other) &&
{
    filters_.push_back(std::move(other));
    return std::move(*this).Erase();
}

std::unique_ptr<EntityFilter> MultiEntityFilter::Combine(std::unique_ptr<EntityFilter> other) const&
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
