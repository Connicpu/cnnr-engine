#pragma once

#include "WatchSystem.h"
#include <LuaInterface/LuaValue.h>

class LuaEntitySystem : public WatchSystem
{
public:
    LuaEntitySystem(int priority, String name, std::unique_ptr<EntityFilter> filter, LuaValue &&table);
    ~LuaEntitySystem();

    virtual void Process(GameData &data) override;
    virtual String GetName() const override;

    static void InitializeLuaModule(lua_State *L);

    bool IsMatch(const GameData &data, Entity e);

    static SystemPtr Build(const LuaValue &meta);

    std::optional<WatchSystem::iterator> &get_iter();

private:
    String name_;
    LuaValue lua_obj_;
    std::optional<WatchSystem::iterator> iter_;
};
