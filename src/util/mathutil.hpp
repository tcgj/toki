#pragma once

#include "system/toki.hpp"

namespace TK {
    template <typename T, typename U>
    inline T lerp(const T &a, const T &b, U t) {
        return (static_cast<U>(1) - t) * a + t * b;
    }

    template <typename T, typename U, typename V>
    inline T clamp(const T &val, const U &lo, const V &hi) {
        if (val < lo)
            return lo;
        else if (val > hi)
            return hi;
        else
            return val;
    }

    inline bool quadratic(tkFloat a, tkFloat b, tkFloat c, tkFloat *t0, tkFloat *t1) {
        tkF64 discriminant = (tkF64)b * (tkF64)b - 4 * (tkF64)a * (tkF64)c;
        if (discriminant < 0)
            return false;

        tkF64 rootDiscriminant = std::sqrt(discriminant);
        tkF64 x = -0.5 * (b < 0 ? b - rootDiscriminant : b + rootDiscriminant);
        *t0 = x / a;
        *t1 = c / x;
        if (*t0 > *t1)
            std::swap(*t0, *t1);

        return true;
    }

    // Obtain tightest interval [i, i + 1] in [0, size) that passes predicate(i)
    template <typename Predicate>
    inline tkInt getInterval(tkInt size, const Predicate &pred) {
        tkInt start = 0, end = size - 1;
        tkInt mid;
        while (start < end) {
            mid = (start + end) >> 1;
            if (pred(mid))
                start = mid + 1;
            else
                end = mid;
        }
        return clamp(start - 1, 0, size - 2);
    }
} // namespace TK
