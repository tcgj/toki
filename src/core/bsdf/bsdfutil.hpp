#pragma once

#include "math/math.hpp"

namespace TK {
    inline tkFloat cosTheta(const tkVec3f &w) {
        return w.y;
    }

    inline tkFloat absCosTheta(const tkVec3f &w) {
        return std::abs(cosTheta(w));
    }

    inline tkFloat cosSqrTheta(const tkVec3f &w) {
        return cosTheta(w) * cosTheta(w);
    }

    inline tkFloat sinSqrTheta(const tkVec3f &w) {
        return std::max((tkFloat)0, 1 - cosSqrTheta(w));
    }

    inline tkFloat sinTheta(const tkVec3f &w) {
        return std::sqrt(sinSqrTheta(w));
    }

    inline tkFloat tanTheta(const tkVec3f &w) {
        return sinTheta(w) / cosTheta(w);
    }

    inline tkFloat tanSqrTheta(const tkVec3f &w) {
        return sinSqrTheta(w) / cosSqrTheta(w);
    }

    inline tkFloat cosPhi(const tkVec3f &w) {
        tkFloat sT = sinTheta(w);
        return sT == 0 ? 1 : clamp(w.x / sT, -1, 1);
    }

    inline tkFloat cosSqrPhi(const tkVec3f &w) {
        return cosPhi(w) * cosPhi(w);
    }

    inline tkFloat sinPhi(const tkVec3f &w) {
        tkFloat sT = sinTheta(w);
        return sT == 0 ? 0 : clamp(w.z / sT, -1, 1);
    }

    inline tkFloat sinSqrPhi(const tkVec3f &w) {
        return sinPhi(w) * sinPhi(w);
    }

    inline bool isSameHemisphere(const tkVec3f &w1, const tkVec3f &w2) {
        return w1.y * w2.y > 0;
    }
} // namespace TK
