#pragma once

#include "System.h"
#include "ComponentId.h"

class ComponentManager;

class TransformUpdate final : public System
{
public:
    TransformUpdate();

    virtual void Process(GameData &data) override;
    virtual String GetName() const override;
};
