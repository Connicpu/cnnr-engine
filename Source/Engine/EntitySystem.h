#pragma once

#include "System.h"
#include <Common/Hash.h>
#include <memory>

class Entity;
class EntityFilter;

class EntitySystem : public System
{
public:
    EntitySystem(std::unique_ptr<EntityFilter> filter);

    virtual void Process(GameData &data) final override;
    virtual void ProcessEntity(GameData &data, Entity e) = 0;

private:
    std::unique_ptr<EntityFilter> filter_;
    HashSet<Entity> watched_;
};
