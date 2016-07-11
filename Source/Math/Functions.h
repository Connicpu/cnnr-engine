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
        return Vec2(lhs / rhs.x, lhs / rhs.y);
    }

    constexpr Vec2F operator/(const Vec2F lhs, const Size2F rhs)
    {
        return Vec2(lhs.x / rhs.width, lhs.y / rhs.height);
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

    inline bool IsInfinite(const Vec2F v)
    {
        return !std::isfinite(LengthSq(v));
    }

    constexpr Vec2F operator-(const Vec2F v)
    {
        return Vec2(-v.x, -v.y);
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

    constexpr Point2F operator-(const Point2F p)
    {
        return Point2(-p.x, -p.y);
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
        return SizeF(lhs / Vec2(rhs));
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
    // Matrix transform functions

    constexpr Matrix3x2F operator*(Matrix3x2F m1, Matrix3x2F m2)
    {
        return Matrix3x2F
        {
            m1.m11 * m2.m11 + m1.m12 * m2.m21,          m1.m11 * m2.m12 + m1.m12 * m2.m22,
            m2.m11 * m1.m21 + m2.m21 * m1.m22,          m2.m12 * m1.m21 + m1.m22 * m2.m22,
            m2.m31 + m2.m11 * m1.m31 + m2.m21 * m1.m32, m2.m32 + m2.m12 * m1.m31 + m2.m22 * m1.m32,
        };
    }

    constexpr Point2F operator*(Matrix3x2F m, Point2F point)
    {
        return Point2(
            point.x*m.m11 + point.y*m.m21 + m.m31,
            point.x*m.m12 + point.y*m.m22 + m.m32
        );
    }

    constexpr Vec2F operator*(Matrix3x2F m, Vec2F vec)
    {
        return Vec2(
            vec.x*m.m11 + vec.y*m.m21,
            vec.x*m.m12 + vec.y*m.m22
        );
    }

    ////////////////////////////
    // Color functions

    constexpr ColorF Color(float r, float g, float b, float a = 1)
    {
        return ColorF{ r, g, b, a };
    }

    constexpr ColorHslF ColorHsl(float h, float s, float l, float a = 1)
    {
        return ColorHslF{ h, s, l, a };
    }

    namespace ColorDetail
    {
        constexpr float hsl_rgb_q(ColorHslF hsl)
        {
            return hsl.l < 0.5f
                ? hsl.l * (1 + hsl.s)
                : hsl.l + hsl.s - hsl.l * hsl.s;
        }

        constexpr float hsl_rgb_p(ColorHslF hsl)
        {
            return hsl.l - hsl_rgb_q(hsl);
        }

        constexpr float hue_to_rgb(float p, float q, float t)
        {
            return t < 0
                ? hue_to_rgb(p, q, t + 1)
                : t > 1
                    ? hue_to_rgb(p, q, t - 1)
                    : t < 1 / 6.f
                        ? p + (q - p) * 6 * t
                        : t < 1 / 2.f
                            ? q
                            : t < 2 / 3.f
                                ? p + (q - p) * 6 * (2 / 3.f - t)
                                : p;
        }

        constexpr bool rgb_max_r(ColorF rgb)
        {
            return rgb.r >= rgb.g && rgb.r >= rgb.b;
        }

        constexpr bool rgb_max_g(ColorF rgb)
        {
            return !rgb_max_r(rgb) && rgb.g >= rgb.b;
        }

        constexpr bool rgb_max_b(ColorF rgb)
        {
            return !rgb_max_r(rgb) && !rgb_max_g(rgb);
        }

        constexpr float rgb_max(ColorF rgb)
        {
            return rgb.r >= rgb.g && rgb.r >= rgb.b
                ? rgb.r
                : rgb.g >= rgb.b
                    ? rgb.g
                    : rgb.b;
        }

        constexpr float rgb_min(ColorF rgb)
        {
            return rgb.r <= rgb.g && rgb.r <= rgb.b
                ? rgb.r
                : rgb.g <= rgb.b
                    ? rgb.g
                    : rgb.b;
        }

        constexpr float rgb_l(ColorF rgb)
        {
            return (rgb_max(rgb) + rgb_min(rgb)) / 2;
        }

        constexpr float rgb_d(ColorF rgb)
        {
            return rgb_max(rgb) - rgb_min(rgb);
        }

        constexpr float rgb_s(ColorF rgb)
        {
            return rgb_l(rgb) > 0.5f
                ? rgb_d(rgb) / (2 - rgb_d(rgb))
                : rgb_d(rgb) / (rgb_max(rgb) + rgb_min(rgb));
        }

        constexpr float rgb_h_r(ColorF rgb)
        {
            return (rgb.g - rgb.b) / rgb_d(rgb) + (rgb.g < rgb.b ? 6 : 0);
        }

        constexpr float rgb_h_g(ColorF rgb)
        {
            return (rgb.b - rgb.r) / rgb_d(rgb) + 2;
        }

        constexpr float rgb_h_b(ColorF rgb)
        {
            return (rgb.r - rgb.g) / rgb_d(rgb) + 4;
        }

        constexpr float rgb_h(ColorF rgb)
        {
            return (rgb_max_r(rgb)
                ? rgb_h_r(rgb)
                : rgb_max_g(rgb)
                    ? rgb_h_g(rgb)
                    : rgb_h_b(rgb))
                / 6.0f;
        }
    }

    constexpr ColorF HslToRgb(ColorHslF hsl)
    {
        using namespace ColorDetail;
        return hsl.s == 0
            ? Color(hsl.l, hsl.l, hsl.l, hsl.a)
            : Color(
                hue_to_rgb(hsl_rgb_p(hsl), hsl_rgb_q(hsl), hsl.h + 1 / 3.f),
                hue_to_rgb(hsl_rgb_p(hsl), hsl_rgb_q(hsl), hsl.h),
                hue_to_rgb(hsl_rgb_p(hsl), hsl_rgb_q(hsl), hsl.h - 1 / 3.f),
                hsl.a
            );
    }

    constexpr ColorHslF RgbToHsl(ColorF rgb)
    {
        using namespace ColorDetail;
        return rgb_max(rgb) == rgb_min(rgb)
            ? ColorHsl(0, 0, rgb_l(rgb))
            : ColorHsl(
                rgb_h(rgb),
                rgb_s(rgb),
                rgb_l(rgb),
                rgb.a
            );
    }
}
