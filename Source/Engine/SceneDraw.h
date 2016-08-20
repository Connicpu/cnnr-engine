#pragma once

#include "System.h"

class SceneDraw : public System
{
public:
    SceneDraw();

    virtual String GetName() const override;
    virtual void Process(GameData &data) override;
};
