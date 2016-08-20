#pragma once

#include "System.h"
#include "EntityFilter.h"

class WatchSystem : public System
{
public:
    WatchSystem(int priority, std::unique_ptr<EntityFilter> filter);

    virtual void OnEntityEvent(const GameData &data, const EntityEvent &event) override;

    using iterator = HashSet<Entity>::const_iterator;
    iterator begin() const;
    iterator end() const;

protected:
    std::unique_ptr<EntityFilter> filter_;
    HashSet<Entity, Fnv1A> watched_;
};
