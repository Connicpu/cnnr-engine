#pragma once

#include "System.h"
#include "ComponentId.h"

class ComponentManager;

class SpriteUpdate final : public System
{
public:
    SpriteUpdate();

    virtual void Process(GameData &data) override;
    virtual String GetName() const override;
};
