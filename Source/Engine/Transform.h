#pragma once

#include "Entity.h"
#include "Component.h"
#include <Common/MathLib.h>

struct GameData;

struct Transform
{
    std::optional<Entity> parent;

    Math::Point2F position;
    Math::Radians rotation;
    Math::Vec2F size;
    float scale;

    Math::Matrix3x2 transform;

    void Update(GameData &data);
};

COMPONENT_NAME(Transform, "transform");
