#pragma once

#include "math/math.hpp"

namespace TK {
    enum class CurveType { FLAT, CYLINDER, RIBBON };

    struct CurveCommon {
        CurveCommon(CurveType type, const tkPoint3f cp[4], tkFloat width0,
                    tkFloat width1, const tkVec3f *n);

        CurveType type;
        tkPoint3f cp[4];
        tkFloat width[2];
        tkVec3f normal[2];
        tkFloat normalAngle, invSinNormalAngle;
    };

    inline CurveCommon::CurveCommon(CurveType type, const tkPoint3f cp[4],
                                    tkFloat width0, tkFloat width1,
                                    const tkVec3f *n)
        : type(type), cp{cp[0], cp[1], cp[2], cp[3]}, width{width0, width1} {
        if (normal) {
            normal[0] = normalize(n[0]);
            normal[1] = normalize(n[1]);
            normalAngle = acos(dot(normal[0], normal[1]));
            invSinNormalAngle = 1.0 / sin(normalAngle);
        }
    }

    inline tkPoint3f bezierBlossom(const tkPoint3f cp[4], tkFloat u0, tkFloat u1, tkFloat u2) {
        tkPoint3f a[3] = {lerp(cp[0], cp[1], u0), lerp(cp[1], cp[2], u0), lerp(cp[2], cp[3], u0)};
        tkPoint3f b[2] = {lerp(a[0], a[1], u1), lerp(a[1], a[2], u1)};
        return lerp(b[0], b[1], u2);
    }

    inline tkPoint3f bezierEval(const tkPoint3f cp[4], tkFloat u, tkVec3f *dp = nullptr) {
        tkPoint3f a[3] = {lerp(cp[0], cp[1], u), lerp(cp[1], cp[2], u), lerp(cp[2], cp[3], u)};
        tkPoint3f b[2] = {lerp(a[0], a[1], u), lerp(a[1], a[2], u)};
        if (dp)
            *dp = (tkFloat)3 * (b[1] - b[0]);
        return lerp(b[0], b[1], u);
    }

    inline void bezierSubdivide(const tkPoint3f cp[4], tkPoint3f cpSplit[7]) {
        cpSplit[0] = cp[0];
        cpSplit[1] = (cp[0] + cp[1]) / 2;
        cpSplit[2] = (cp[0] + 2 * cp[1] + cp[2]) / 4;
        cpSplit[3] = (cp[0] + 3 * cp[1] + 3 * cp[2] + cp[3]) / 8;
        cpSplit[4] = (cp[1] + 2 * cp[2] + cp[3]) / 4;
        cpSplit[5] = (cp[2] + cp[3]) / 2;
        cpSplit[6] = cp[3];
    }
} // namespace TK