#pragma once

#include "Entity.h"
#include "Component.h"
#include <Common/MathLib.h>

struct GameData;

struct Transform
{
    std::optional<Entity> parent;

    Math::Point2F position = Math::Point2();
    Math::Radians rotation = Math::Rads(0);
    Math::Vec2F size = Math::Vec2();
    float scale = 1.f;

    Math::Matrix3x2 transform;
    bool changed = true;

    void Update(GameData &data);

private:
    bool updating;
};

COMPONENT_NAME(Transform, "transform");
