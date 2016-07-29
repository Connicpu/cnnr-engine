#include "EntitySystem.h"
#include "Entity.h"

EntitySystem::EntitySystem(std::unique_ptr<EntityFilter> filter)
    : filter_(std::move(filter))
{
}

void EntitySystem::Process(GameData &data)
{
    for (Entity entity : watched_)
    {
        ProcessEntity(data, entity);
    }
}
