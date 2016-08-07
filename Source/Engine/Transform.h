#pragma once

#include "Entity.h"
#include "Component.h"
#include <Common/MathLib.h>

struct GameData;

struct Transform : public Component
{
    std::optional<Entity> parent;

    Math::Point2F position = Math::Point2();
    Math::Radians rotation = Math::Rads(0);
    Math::Size2F size = Math::SizeF();
    float scale = 1.f;

    Math::Matrix3x2 transform;
    bool changed = true;

    void Update(GameData &data);

    virtual String GetName() const;
    virtual void PushLuaBinding(lua_State *L);
    static std::optional<Transform &> FromLua(lua_State *L, int idx);

private:
    static void PushTransformMetatable(lua_State *L);

    bool updating;
};
