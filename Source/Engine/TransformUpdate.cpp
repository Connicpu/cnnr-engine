#include "pch.h"
#include "TransformUpdate.h"
#include "EntityFilter.h"
#include "ComponentManager.h"
#include "GameData.h"

TransformUpdate::TransformUpdate(const ComponentManager &components)
    : EntitySystem(1000, component_list_filter(components, "transform"_s))
{
}

void TransformUpdate::ProcessEntity(GameData &data, Entity e)
{
    data.components.transform[e].Update(data);
}

String TransformUpdate::GetName() const
{
    return "TransformUpdate"_s;
}

