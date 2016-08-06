#pragma once

#include "EntitySystem.h"

class ComponentManager;

class TransformUpdate final : public EntitySystem
{
public:
    TransformUpdate(const ComponentManager &components);

    virtual void ProcessEntity(GameData &data, Entity e) override;
    virtual String GetName() const override;
};
