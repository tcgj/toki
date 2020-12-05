#pragma once

#include "system/toki.hpp"

namespace TK {
    inline tkFloat cosTheta(const Vec3f& w) {
        return w.z;
    }

    inline tkFloat absCosTheta(const Vec3f& w) {
        return std::abs(cosTheta(w));
    }

    inline tkFloat cosSqrTheta(const Vec3f& w) {
        return cosTheta(w) * cosTheta(w);
    }

    inline tkFloat sinSqrTheta(const Vec3f& w) {
        return std::max((tkFloat)0, 1 - cosSqrTheta(w));
    }

    inline tkFloat sinTheta(const Vec3f& w) {
        return std::sqrt(sinSqrTheta(w));
    }

    inline tkFloat tanTheta(const Vec3f& w) {
        return sinTheta(w) / cosTheta(w);
    }

    inline tkFloat tanSqrTheta(const Vec3f& w) {
        return sinSqrTheta(w) / cosSqrTheta(w);
    }

    inline tkFloat cosPhi(const Vec3f& w) {
        tkFloat sT = sinTheta(w);
        return sT == 0 ? 1 : clamp(w.x / sT, -1, 1);
    }

    inline tkFloat cosSqrPhi(const Vec3f& w) {
        return cosPhi(w) * cosPhi(w);
    }

    inline tkFloat sinPhi(const Vec3f& w) {
        tkFloat sT = sinTheta(w);
        return sT == 0 ? 0 : clamp(w.y / sT, -1, 1);
    }

    inline tkFloat sinSqrPhi(const Vec3f& w) {
        return sinPhi(w) * sinPhi(w);
    }

    inline bool isSameHemisphere(const Vec3f& w1, const Vec3f& w2) {
        return w1.z * w2.z > 0;
    }

    inline Vec3f reflect(const Vec3f& v, const Vec3f& n) {
        return 2 * dot(v, n) * n - v;
    }

    inline Vec3f refract(const Vec3f& v, const Vec3f& n, tkFloat etaI, tkFloat etaT) {
        tkFloat eta = etaI / etaT;
        tkFloat cosI = dot(v, n);
        tkFloat sinSqrI = std::max((tkFloat)0, 1 - cosI * cosI);
        tkFloat sinSqrT = eta * eta * sinSqrI;
        if (sinSqrT >= 1)
            return Vec3f::zero;

        tkFloat cosT = std::sqrt(1 - sinSqrT);
        return -eta * v + (eta * cosI - cosT) * n;
    }
}  // namespace TK
