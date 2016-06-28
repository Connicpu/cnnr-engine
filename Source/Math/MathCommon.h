#pragma once

#include <limits>   

namespace MathDetail
{
    double constexpr sqrt_newton_raphson(double x, double curr, double prev)
    {
        return curr == prev
            ? curr
            : sqrt_newton_raphson(x, 0.5 * (curr + x / curr), curr);
    }
}

double constexpr constexpr_sqrt(double x)
{
    return x >= 0 && x < std::numeric_limits<double>::infinity()
        ? MathDetail::sqrt_newton_raphson(x, x, 0)
        : std::numeric_limits<double>::quiet_NaN();
}

#ifdef MSVC
#define MCONV __vectorcall
#else
#define MCONV
#endif
