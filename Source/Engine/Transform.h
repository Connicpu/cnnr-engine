#pragma once

#include "Entity.h"
#include "Component.h"
#include <Common/MathLib.h>

struct GameData;

struct Transform : public Component
{
    Math::Matrix3x2 transform;
    Math::Matrix3x2 parallel_calc;

    Math::Point2F position = Math::Point2();
    Math::Radians rotation = Math::Rads(0);
    float scale = 1.f;
    Math::Size2F size = Math::SizeF();

    std::optional<Entity> parent;
    bool changed = true;

    void Update(GameData &data);
    void ParallelCalculate();
    void ParallelUpdate(GameData &data);

    virtual String GetName() const;
    virtual void PushLuaBinding(lua_State *L);
    static std::optional<Transform &> FromLua(lua_State *L, int idx);

private:
    static void PushTransformMetatable(lua_State *L);
};
