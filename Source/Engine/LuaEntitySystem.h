#pragma once

#include "WatchSystem.h"
#include <LuaInterface/LuaValue.h>

class LuaEntitySystem : public WatchSystem
{
public:
    LuaEntitySystem(int priority, String name, std::unique_ptr<EntityFilter> filter, const LuaValue &meta);
    ~LuaEntitySystem();

    virtual void Process(GameData &data) override;
    virtual String GetName() const override;

    static void InitializeLuaModule(lua_State *L);

private:
    String name_;
    LuaValue lua_obj_;
};
