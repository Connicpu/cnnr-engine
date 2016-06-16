#pragma once

#include "RenderBase.h"
#include "Target.h"
#include "Events.h"
#include <memory>

class IDisplay : public IRenderTarget
{
public:
    virtual bool PollEvent(Event **event) = 0;
    virtual void Present() = 0;

    ///////////////////////
    // Inline helpers

    inline bool PollEvent(EventPtr &event)
    {
        Event *ptr;
        bool result = this->PollEvent(&ptr);
        if (result)
            event = EventPtr{ ptr, EventFree(&free) };
        return result;
    }
};
