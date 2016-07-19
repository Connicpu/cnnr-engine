#pragma once

#include "Structures.h"
#include "Functions.h"
#include <Common/optional.h>
#include <initializer_list>
#include <cassert>
#include <cstring>
#include <stdexcept>

namespace Math
{
    struct Matrix3x2 : Matrix3x2F
    {
        constexpr Matrix3x2();
        constexpr Matrix3x2(const Matrix3x2F &mat);

        constexpr static Matrix3x2 Identity();
        constexpr static Matrix3x2 Translation(Vec2F vec);
        constexpr static Matrix3x2 Translation(Point2F point);
        constexpr static Matrix3x2 Translation(float x, float y);
        constexpr static Matrix3x2 Scale(Size2F scale, Point2F center = Point2());
        inline static Matrix3x2 Rotation(Radians angle, Point2F center = Point2());
        inline static Matrix3x2 Rotation(Degrees angle, Point2F center = Point2());
        constexpr static Matrix3x2 RotationCx(Radians angle, Point2F center = Point2());
        constexpr static Matrix3x2 RotationCx(Degrees angle, Point2F center = Point2());
        inline static Matrix3x2 Skew(Radians angle_x, Radians angle_y, Point2F center = Point2());
        inline static Matrix3x2 Skew(Degrees angle_x, Degrees angle_y, Point2F center = Point2());
        constexpr static Matrix3x2 SkewCx(Radians angle_x, Radians angle_y, Point2F center = Point2());
        constexpr static Matrix3x2 SkewCx(Degrees angle_x, Degrees angle_y, Point2F center = Point2());

        constexpr float Determinant() const;
        constexpr bool IsInvertible() const;
        constexpr Matrix3x2F InverseCx() const;
        inline bool Inverse(Matrix3x2F &result) const;
        inline std::optional<Matrix3x2F> Inverse() const;
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

inline constexpr Math::Matrix3x2 Math::Matrix3x2::Translation(float x, float y)
{
    return Translation(Vec2(x, y));
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

inline Math::Matrix3x2 Math::Matrix3x2::Rotation(Radians angle, Point2F center)
{
    const float cos = std::cos(angle.rad);
    const float sin = std::sin(angle.rad);
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

inline Math::Matrix3x2 Math::Matrix3x2::Rotation(Degrees angle, Point2F center)
{
    return Matrix3x2::Rotation(Rads(angle), center);
}

constexpr Math::Matrix3x2 Math::Matrix3x2::RotationCx(Radians angle, Point2F center)
{
    return Matrix3x2F
    {
        cx::cos(angle.rad), -cx::sin(angle.rad),
        cx::sin(angle.rad),  cx::cos(angle.rad),
        center.x - cx::cos(angle.rad)*center.x - cx::sin(angle.rad)*center.y,
        center.y - cx::cos(angle.rad)*center.y - cx::sin(angle.rad)*center.x,
    };
}

inline constexpr Math::Matrix3x2 Math::Matrix3x2::RotationCx(Degrees angle, Point2F center)
{
    return Matrix3x2::RotationCx(Rads(angle), center);
}

inline Math::Matrix3x2 Math::Matrix3x2::Skew(Radians angle_x, Radians angle_y, Point2F center)
{
    const float tanx = std::tan(angle_x.rad);
    const float tany = std::tan(angle_y.rad);
    const float x = center.x, y = center.y;

    return Matrix3x2F
    {
           1.0f,    tanx,
           tany,    1.0f,
        -y*tany, -x*tanx,
    };
}

inline Math::Matrix3x2 Math::Matrix3x2::Skew(Degrees angle_x, Degrees angle_y, Point2F center)
{
    return Matrix3x2::Skew(Rads(angle_x), Rads(angle_y), center);
}

constexpr Math::Matrix3x2 Math::Matrix3x2::SkewCx(Radians angle_x, Radians angle_y, Point2F center)
{
    return Matrix3x2F
    {
        1.0f, cx::tan(angle_x.rad),
        cx::tan(angle_y.rad), 1.0f,
        -center.y * cx::tan(angle_y.rad), -center.x * cx::tan(angle_x.rad),
    };
}

inline constexpr Math::Matrix3x2 Math::Matrix3x2::SkewCx(Degrees angle_x, Degrees angle_y, Point2F center)
{
    return Matrix3x2::SkewCx(Rads(angle_x), Rads(angle_y), center);
}

constexpr float Math::Matrix3x2::Determinant() const
{
    return m11 * m22 - m12 * m21;
}

constexpr bool Math::Matrix3x2::IsInvertible() const
{
    return cx::abs(Determinant()) > 0;
}

constexpr Math::Matrix3x2F Math::Matrix3x2::InverseCx() const
{
    return !IsInvertible()
        ? (throw std::logic_error{ "Inverse attempted of non-invertible matrix as a constexpr" })
        : Matrix3x2F
    {
        m22 /  Determinant(), m12 / -Determinant(),
        m21 / -Determinant(), m11 /  Determinant(),

        (m22*m31 - m21*m32) / -Determinant(),
        (m12*m31 - m11*m32) /  Determinant(),
    };
}

inline bool Math::Matrix3x2::Inverse(Matrix3x2F &result) const
{
    if (!IsInvertible())
        return false;

    const float det = Determinant();

    result =
    {
        m22 /  det, m12 / -det,
        m21 / -det, m11 /  det,
        (m22*m31 - m21*m32) / -det,
        (m12*m31 - m11*m32) /  det,
    };

    return true;
}

inline std::optional<Math::Matrix3x2F> Math::Matrix3x2::Inverse() const
{
    Matrix3x2F temp;
    if (Inverse(temp))
        return temp;
    return{};
}

