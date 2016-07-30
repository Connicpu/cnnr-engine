#include "EntitySystem.h"
#include "Entity.h"
#include "EntityEvent.h"

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

void EntitySystem::OnEntityEvent(const GameData &data, const EntityEvent &event)
{
    switch (event.action)
    {
        case EntityEvent::EntityAdded:
        case EntityEvent::EntityModified:
        {
            if (filter_->IsMatch(data, event.entity))
            {
                watched_.insert(event.entity);
            }
            else
            {
                watched_.erase(event.entity);
            }
            break;
        }
        case EntityEvent::EntityRemoved:
        {
            watched_.erase(event.entity);
            break;
        }
        default: unreachable();
    }
}
