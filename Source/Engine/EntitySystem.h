#pragma once

#include "System.h"
#include <Common/Hash.h>
#include <memory>

class Entity;

class EntityFilter
{
public:
    virtual ~EntityFilter() {}
    virtual bool IsMatch(const GameData &data, Entity e) const = 0;
};

class EntitySystem : public System
{
public:
    EntitySystem(int priority, std::unique_ptr<EntityFilter> filter);

    virtual void Process(GameData &data) final override;
    virtual void OnEntityEvent(const GameData &data, const EntityEvent &event) override;

    virtual void ProcessEntity(GameData &data, Entity e) = 0;

private:
    std::unique_ptr<EntityFilter> filter_;
    HashSet<Entity> watched_;
};
