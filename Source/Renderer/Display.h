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

    inline bool PollEvent(std::unique_ptr<Event> &event)
    {
        Event *ptr;
        bool result = this->PollEvent(&ptr);
        if (result)
            event = std::unique_ptr<Event>{ ptr };
        return result;
    }
};
