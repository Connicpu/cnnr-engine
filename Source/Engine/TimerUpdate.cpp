#include "pch.h"
#include "TimerUpdate.h"
#include "GameData.h"
#include <Common/RawTime.h>

TimerUpdate::TimerUpdate()
    : System(0)
{
    resolution_ = rawtime_resolution();
    start_time_ = rawtime_now();
    last_time_ = start_time_;
}

String TimerUpdate::GetName() const
{
    return "TimerUpdate"_s;
}

void TimerUpdate::Process(GameData &data)
{
    auto now = rawtime_now();
    data.services.delta = double(now - last_time_) / resolution_;
    data.services.running_time = double(now - start_time_) / resolution_;
    last_time_ = now;
}
