#pragma once

class String;
class GameData;
struct EntityEvent;

class System
{
public:
    virtual String GetName() = 0;
    virtual void Process(GameData &data) = 0;
    virtual void OnEntityEvent(const EntityEvent &event);
};
