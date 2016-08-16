#include "pch.h"
#include "WindowHandler.h"
#include "GameData.h"

WindowHandler::WindowHandler()
    : System(100)
{
}

String WindowHandler::GetName() const
{
    return "WindowHandler"_s;
}

void WindowHandler::Process(GameData &data)
{
    auto display = data.services.graphics->GetDisplay();
    EventStorage event;
    
    while (display->PollEvent(event))
    {
        switch (event.base.type)
        {
            case EventType::Resized:
                //TODO
                break;
            case EventType::Moved:
                //TODO
                break;
            case EventType::Closed:
                //TODO: Maybe make a lua callback so we can show a confirmation of destructive action?
                // Or should Alt+F4 be an instant exit no matter what?
                data.services.running = false;
                break;
            case EventType::DroppedFile:
                //TODO
                break;
            case EventType::ReceivedCharacter:
                //TODO
                break;
            case EventType::Focused:
                //TODO
                break;
            case EventType::KeyboardInput:
                //TODO
                break;
            case EventType::MouseMoved:
                //TODO
                break;
            case EventType::MouseWheel:
                //TODO
                break;
            case EventType::MouseInput:
                //TODO
                break;
            case EventType::Touch:
                //TODO
                break;
        }
    }
}
