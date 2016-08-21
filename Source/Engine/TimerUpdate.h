#pragma once

#include "System.h"
#include <Common/Platform.h>

class TimerUpdate : public System
{
public:
    TimerUpdate();

    virtual String GetName() const override;
    virtual void Process(GameData &data) override;

private:
    uint64_t start_time_;
    uint64_t last_time_;
    uint64_t resolution_;
};
