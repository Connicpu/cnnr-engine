#include "pch.h"
#include "WatchSystem.h"
#include "EntityEvent.h"

WatchSystem::WatchSystem(int priority, std::unique_ptr<EntityFilter> filter)
    : System(priority), filter_(std::move(filter))
{
}

void WatchSystem::OnEntityEvent(const GameData &data, const EntityEvent &event)
{
    switch (event.action)
    {
        case EntityEvent::EntityAdded:
        case EntityEvent::EntityModified:
        {
            if (filter_->CanMatch(data, event.entity))
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

HashSet<Entity>::const_iterator WatchSystem::begin() const
{
    return watched_.begin();
}

HashSet<Entity>::const_iterator WatchSystem::end() const
{
    return watched_.end();
}
