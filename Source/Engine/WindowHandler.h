#pragma once

#include "System.h"

class WindowHandler : public System
{
public:
    WindowHandler();

    virtual String GetName() const override;
    virtual void Process(GameData &data) override;
};
