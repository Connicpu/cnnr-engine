#pragma once

#include "Structures.h"
#include "Functions.h"
#include <initializer_list>
#include <cassert>
#include <cstring>

namespace Math
{
    struct Matrix3x2 : Matrix3x2F
    {
        constexpr Matrix3x2();
        constexpr Matrix3x2(const Matrix3x2F &mat);

        constexpr static Matrix3x2 Identity();
        constexpr static Matrix3x2 Translation(Vec2F vec);
        constexpr static Matrix3x2 Translation(Point2F point);
        constexpr static Matrix3x2 Scale(Size2F scale, Point2F center = Point2());
        inline static Matrix3x2 Rotation(float angle, Point2F center = Point2());
        constexpr static Matrix3x2 RotationCx(float angle, Point2F center = Point2());
    };
}

constexpr Math::Matrix3x2::Matrix3x2()
    : Matrix3x2F { 0 } // zero-extend
{
}

constexpr Math::Matrix3x2::Matrix3x2(const Matrix3x2F &mat)
    : Matrix3x2F(mat)
{
}

constexpr Math::Matrix3x2 Math::Matrix3x2::Identity()
{
    return Matrix3x2F
    {
        1, 0,
        0, 1,
        0, 0,
    };
}

constexpr Math::Matrix3x2 Math::Matrix3x2::Translation(const Vec2F vec)
{
    return Matrix3x2F
    {
        1, 0,
        0, 1,
        vec.x, vec.y,
    };
}

constexpr Math::Matrix3x2 Math::Matrix3x2::Translation(const Point2F point)
{
    return Translation(Vec2(point));
}

constexpr Math::Matrix3x2 Math::Matrix3x2::Scale(const Size2F scale, const Point2F center)
{
    return Matrix3x2F
    {
        scale.width, 0,
        0, scale.height,
        center.x - scale.width * center.x, center.y - scale.height * center.y,
    };
}

inline Math::Matrix3x2 Math::Matrix3x2::Rotation(float angle, Point2F center)
{
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);
    const float x = center.x;
    const float y = center.y;
    const float tx = x - cos*x - sin*y;
    const float ty = y - cos*y - sin*x;

    return Matrix3x2F
    {
        cos, -sin,
        sin,  cos,
         tx,   ty,
    };
}

constexpr Math::Matrix3x2 Math::Matrix3x2::RotationCx(float angle, Point2F center)
{
    return Matrix3x2F
    {
        cx::cos(angle), -cx::sin(angle),
        cx::sin(angle),  cx::cos(angle),
        center.x - cx::cos(angle)*center.x - cx::sin(angle)*center.y,
        center.y - cx::cos(angle)*center.y - cx::sin(angle)*center.x,
    };
}

