#pragma once

#include "math/math.hpp"
#include "transform.hpp"

namespace TK {
    struct Quaternion {
        Quaternion() : xyz(0, 0, 0), w(1) {}
        Quaternion(const Transform &t);

        // Unary operators
        Quaternion operator-() const;

        // Assignment operators
        Quaternion &operator+=(const Quaternion &q);
        Quaternion &operator-=(const Quaternion &q);
        Quaternion &operator*=(tkFloat f);
        Quaternion &operator/=(tkFloat f);

        // Binary operators
        Quaternion operator+(const Quaternion &q) const;
        Quaternion operator-(const Quaternion &q) const;
        Quaternion operator*(tkFloat f) const;
        Quaternion operator/(tkFloat f) const;

        Quaternion &normalized();
        Transform toTransform() const;

        tkVec3f xyz;
        tkFloat w;
    };

    Quaternion slerp(tkFloat t, const Quaternion &q1, const Quaternion &q2);

    inline Quaternion Quaternion::operator-() const {
        Quaternion ret;
        ret.xyz = -xyz;
        ret.w = -w;
        return ret;
    }

    inline Quaternion &Quaternion::operator+=(const Quaternion &q) {
        xyz += q.xyz;
        w += q.w;
        return *this;
    }
    inline Quaternion &Quaternion::operator-=(const Quaternion &q) {
        xyz -= q.xyz;
        w -= q.w;
        return *this;
    }
    inline Quaternion &Quaternion::operator*=(tkFloat f) {
        xyz *= f;
        w *= f;
        return *this;
    }
    inline Quaternion &Quaternion::operator/=(tkFloat f) {
        tkFloat r = (tkFloat)1 / f;
        xyz *= r;
        w *= r;
        return *this;
    }

    inline Quaternion Quaternion::operator+(const Quaternion &q) const {
        Quaternion ret = *this;
        return ret += q;
    }
    inline Quaternion Quaternion::operator-(const Quaternion &q) const {
        Quaternion ret = *this;
        return ret -= q;
    }
    inline Quaternion Quaternion::operator*(tkFloat f) const {
        Quaternion ret = *this;
        ret.xyz *= f;
        ret.w *= f;
        return ret;
    }
    inline Quaternion Quaternion::operator/(tkFloat f) const {
        Quaternion ret = *this;
        tkFloat r = (tkFloat)1 / f;
        ret.xyz *= r;
        ret.w *= r;
        return ret;
    }

    inline Quaternion &Quaternion::normalized() {
        tkFloat sm = dot(*this, *this);
        if (sm > 0) {
            *this /= sqrt(sm);
        }
        return *this;
    }

    // Binary operators
    inline Quaternion operator*(tkFloat f, const Quaternion &q) {
        Quaternion ret = q;
        ret.xyz *= f;
        ret.w *= f;
        return ret;
    }

    // Quaternion operations
    inline tkFloat dot(const Quaternion &q1, const Quaternion &q2) {
        return dot(q1.xyz, q2.xyz) + q1.w * q2.w;
    }
    inline Quaternion normalize(const Quaternion &q) {
        tkFloat sm = dot(q, q);
        if (sm > 0) {
            return q / sqrt(sm);
        }
        return q;
    }
} // namespace TK