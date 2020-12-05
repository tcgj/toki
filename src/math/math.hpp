#pragma once

#define TK_PI 3.141592653589793
#define TK_INVPI 0.31830988618
#define TK_INV2PI 0.15915494309
#define TK_INV4PI 0.07957747154
#define TK_PIOVER2 1.57079632679
#define TK_PIOVER4 0.78539816339

// Used for small offsets, e.g. shadow epsilon
#define TK_EPSILON (tkFloat)0.00001
// Represents the largest float below 1
#ifdef TK_USE_DOUBLE
#define TK_ONE_MINUS_EPSILON 0x1.fffffffffffffp-1
#else
#define TK_ONE_MINUS_EPSILON 0x1.fffffep-1f
#endif

#define degToRad(x) x * TK_PI / 180.0
#define radToDeg(x) x * 180.0 / TK_PI

namespace TK {
    // Constants
    constexpr tkFloat TK_MAXFLOAT = std::numeric_limits<tkFloat>::max();
    constexpr tkFloat TK_INFINITY = std::numeric_limits<tkFloat>::infinity();

    // Generic math functions
    template <typename T, typename U>
    inline T lerp(const T& a, const T& b, U t) {
        return (static_cast<U>(1) - t) * a + t * b;
    }

    template <typename T, typename U, typename V>
    inline T clamp(const T& val, const U& lo, const V& hi) {
        if (val < lo)
            return lo;
        else if (val > hi)
            return hi;
        else
            return val;
    }

    inline bool quadratic(tkFloat a, tkFloat b, tkFloat c, tkFloat* t0, tkFloat* t1) {
        double discriminant = (double)b * (double)b - 4 * (double)a * (double)c;
        if (discriminant < 0)
            return false;

        double rootDiscriminant = std::sqrt(discriminant);
        double x = -0.5 * (b < 0 ? b - rootDiscriminant : b + rootDiscriminant);
        *t0 = x / a;
        *t1 = c / x;
        if (*t0 > *t1)
            std::swap(*t0, *t1);

        return true;
    }

    // Obtain tightest interval [i, i + 1] in [0, size) that passes predicate(i)
    template <typename Predicate>
    inline int getInterval(int size, const Predicate& pred) {
        int start = 0, end = size - 1;
        int mid;
        while (start < end) {
            mid = (start + end) >> 1;
            if (pred(mid))
                start = mid + 1;
            else
                end = mid;
        }
        return clamp(start - 1, 0, size - 2);
    }
}  // namespace TK
