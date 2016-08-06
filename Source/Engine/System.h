#pragma once

#include <Common/MaybeOwned.h>

class String;
struct GameData;
struct EntityEvent;

class System
{
public:
    System(int priority)
        : priority(priority)
    {
    }

    inline virtual ~System() {};

    virtual String GetName() const = 0;
    virtual void Process(GameData &data) = 0;
    virtual void OnEntityEvent(const GameData &data, const EntityEvent &event);

    int priority;
};

using SystemPtr = MaybeOwned<System>;
