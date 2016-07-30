#pragma once

#include <Common/MaybeOwned.h>

class String;
struct GameData;
struct EntityEvent;

class System
{
public:
    virtual String GetName() = 0;
    virtual void Process(GameData &data) = 0;
    virtual void OnEntityEvent(const GameData &data, const EntityEvent &event);
};

using SystemPtr = MaybeOwned<System>;
