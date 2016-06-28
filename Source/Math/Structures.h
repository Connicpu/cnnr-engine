#pragma once

#include <stdint.h>

struct Vec2F
{
    float x, y;
};

struct Point2F
{
    float x, y;
};

struct Size2F
{
    float width, height;
};

struct Size2U
{
    uint32_t width, height;
};

struct RectF
{
    float left, top, right, bottom;
};

struct Matrix3x2F
{
    float m11, m12;
    float m21, m22;
    float m31, m32;
};

struct ColorF
{
    float r, g, b, a;
};
