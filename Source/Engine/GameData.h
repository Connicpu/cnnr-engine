#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "ServiceManager.h"

struct GameData
{
    GameData();

    EntityManager entities;
    ComponentManager components;
    SystemManager systems;
    ServiceManager services;
};
