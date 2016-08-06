#pragma once

#include "WatchSystem.h"
#include <Common/Hash.h>

class EntitySystem : public WatchSystem
{
public:
    EntitySystem(int priority, std::unique_ptr<EntityFilter> filter);

    virtual void Process(GameData &data) final override;
    virtual void ProcessEntity(GameData &data, Entity e) = 0;
};
