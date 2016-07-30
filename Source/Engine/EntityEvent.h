#pragma once

#include "Entity.h"
#include <Common/String.h>

struct EntityEvent
{
    enum Action { EntityAdded, EntityModified, EntityRemoved };

    EntityEvent(Action action, Entity entity)
        : action(action), entity(entity)
    {
    }

    Action action;
    Entity entity;
};
