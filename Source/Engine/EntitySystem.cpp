#include "pch.h"
#include "EntitySystem.h"
#include "Entity.h"
#include "EntityEvent.h"

EntitySystem::EntitySystem(int priority, std::unique_ptr<EntityFilter> filter)
    : WatchSystem(priority, std::move(filter))
{
}

void EntitySystem::Process(GameData &data)
{
    for (Entity entity : watched_)
    {
        ProcessEntity(data, entity);
    }
}
