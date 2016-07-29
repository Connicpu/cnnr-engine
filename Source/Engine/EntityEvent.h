#pragma once

#include "Entity.h"
#include <Common/String.h>

struct EntityEvent
{
    enum Action { EntityAdded, EntityModified, EntityRemoved };
    Action action;
    Entity entity;
};
