#pragma once

#include "Structures.h"
#include "CxMath.h"
#include <cmath>

namespace Math
{
    ////////////////////////////
    // Vec2 functions

    constexpr Vec2F Vec2()
    {
        return Vec2F{ 0.f, 0.f };
    }

    constexpr Vec2F Vec2(float x, float y)
    {
        return Vec2F{ x, y };
    }

    constexpr Vec2F Vec2(const Point2F p)
    {
        return Vec2(p.x, p.y);
    }

    constexpr Vec2F Vec2(const Size2F s)
    {
        return Vec2(s.width, s.height);
    }

    constexpr Vec2F operator+(const Vec2F lhs, const Vec2F rhs)
    {
        return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    constexpr Vec2F operator-(const Vec2F lhs, const Vec2F rhs)
    {
        return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    constexpr Vec2F operator*(const Vec2F lhs, float rhs)
    {
        return Vec2(lhs.x * rhs, lhs.y * rhs);
    }

    constexpr Vec2F operator*(float lhs, const Vec2F rhs)
    {
        return rhs * lhs;
    }

    constexpr Vec2F operator/(const Vec2F lhs, float rhs)
    {
        return Vec2(lhs.x / rhs, lhs.y / rhs);
    }

    constexpr Vec2F operator/(float lhs, const Vec2F rhs)
    {
        return rhs * lhs;
    }

    constexpr float Dot(const Vec2F v1, const Vec2F v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    constexpr float LengthSq(const Vec2F v)
    {
        return Dot(v, v);
    }

    inline float Length(const Vec2F v)
    {
        return std::sqrt(LengthSq(v));
    }
    constexpr float LengthCx(const Vec2F v)
    {
        return cx::sqrt(LengthSq(v));
    }

    inline Vec2F Normalize(const Vec2F v)
    {
        return v / Length(v);
    }
    constexpr Vec2F NormalizeCx(const Vec2F v)
    {
        return v / LengthCx(v);
    }

    constexpr bool IsZero(const Vec2F v)
    {
        return LengthSq(v) < 1e-8;
    }

    ////////////////////////////
    // Point2 functions

    constexpr Point2F Point2()
    {
        return Point2F{ 0.f, 0.f };
    }

    constexpr Point2F Point2(float x, float y)
    {
        return Point2F{ x, y };
    }

    constexpr Point2F Point2(const Vec2F v)
    {
        return Point2(v.x, v.y);
    }

    constexpr Point2F operator+(const Point2F lhs, const Vec2F rhs)
    {
        return Point2(Vec2(lhs) + rhs);
    }

    constexpr Point2F operator+(const Vec2F lhs, const Point2F rhs)
    {
        return Point2(lhs + Vec2(rhs));
    }

    constexpr Point2F operator-(const Point2F lhs, const Vec2F rhs)
    {
        return Point2(Vec2(lhs) - rhs);
    }

    constexpr Vec2F operator-(const Point2F lhs, const Point2F rhs)
    {
        return Vec2(lhs) - Vec2(rhs);
    }

    ////////////////////////////
    // SizeF functions

    constexpr Size2F SizeF()
    {
        return Size2F{ 0.f, 0.f };
    }

    constexpr Size2F SizeF(float width, float height)
    {
        return Size2F{ width, height };
    }

    constexpr Size2F SizeF(const Vec2F v)
    {
        return SizeF(v.x, v.y);
    }

    constexpr Size2F operator+(const Size2F lhs, const Size2F rhs)
    {
        return SizeF(Vec2(lhs) + Vec2(rhs));
    }

    constexpr Size2F operator-(const Size2F lhs, const Size2F rhs)
    {
        return SizeF(Vec2(lhs) - Vec2(rhs));
    }

    constexpr Size2F operator*(const Size2F lhs, float rhs)
    {
        return SizeF(Vec2(lhs) * rhs);
    }

    constexpr Size2F operator*(float lhs, const Size2F rhs)
    {
        return rhs * lhs;
    }

    constexpr Size2F operator/(const Size2F lhs, float rhs)
    {
        return SizeF(Vec2(lhs) / rhs);
    }

    constexpr Size2F operator/(float lhs, const Size2F rhs)
    {
        return rhs / lhs;
    }

    ////////////////////////////
    // SizeU functions

    constexpr Size2U SizeU()
    {
        return Size2U{ 0, 0 };
    }

    constexpr Size2U SizeU(uint32_t width, uint32_t height)
    {
        return Size2U{ width, height };
    }

    constexpr Size2U operator+(const Size2U lhs, const Size2U rhs)
    {
        return SizeU(lhs.width + rhs.width, lhs.height + rhs.height);
    }

    constexpr Size2U operator-(const Size2U lhs, const Size2U rhs)
    {
        return SizeU(lhs.width - rhs.width, lhs.height - rhs.height);
    }

    ////////////////////////////
    // Rect functions

    constexpr RectF Rect(float left, float top, float right, float bottom)
    {
        return RectF{ left, top, right, bottom };
    }

    constexpr RectF Rect()
    {
        return Rect(0, 0, 0, 0);
    }

    constexpr RectF Rect(float x, float y)
    {
        return Rect(x, y, x, y);
    }

    constexpr RectF Rect(Point2F top_left, Size2F size)
    {
        return Rect(top_left.x, top_left.y, top_left.x + size.width, top_left.y + size.height);
    }

    constexpr RectF operator+(const RectF rect, const Size2F size)
    {
        return Rect(rect.left, rect.top, rect.right + size.width, rect.bottom + size.height);
    }

    constexpr RectF operator+(const RectF rect, const Vec2F trans)
    {
        return Rect(rect.left + trans.x, rect.top + trans.y, rect.right + trans.x, rect.bottom + trans.y);
    }

    constexpr RectF operator-(const RectF rect, const Size2F size)
    {
        return Rect(rect.left, rect.top, rect.right - size.width, rect.bottom - size.height);
    }

    constexpr RectF operator-(const RectF rect, const Vec2F trans)
    {
        return Rect(rect.left - trans.x, rect.top - trans.y, rect.right - trans.x, rect.bottom - trans.y);
    }

    ////////////////////////////
    // Color functions
}
