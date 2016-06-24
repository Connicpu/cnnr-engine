#pragma once

#include "RenderBase.h"
#include "Target.h"
#include "Events.h"
#include <memory>

class IDisplay : public IRenderTarget
{
public:
    virtual bool PollEvent(EventPtr &event) = 0;
    virtual void Present() = 0;
    virtual bool Closed() = 0;
};
