#pragma once

#include "math/math.hpp"
#include "ray.hpp"

namespace TK {
    class Transform {
    public:
        Transform() = default;
        Transform(const tkFloat d[16]) {
            m = Matrix44(d[0], d[1], d[2], d[3],
                         d[4], d[5], d[6],  d[7],
                         d[8], d[9],  d[10], d[11],
                         d[12],  d[13], d[14], d[15]);
            mInv = inverse(m);
        }
        Transform(const Matrix44 &m) : m(m), mInv(inverse(m)) {}
        Transform(const Matrix44 &m, const Matrix44 &mInv) : m(m), mInv(mInv) {}

        // Transform Application
        template <typename T> Point3<T> operator()(const Point3<T> &p) const;
        template <typename T> Vec3<T> operator()(const Vec3<T> &v, bool isNormal = false) const;
        template <typename T> AABB<T> operator()(const AABB<T> &bb) const;
        Ray operator()(const Ray &r) const;

        // Equality
        bool operator==(const Transform &t) const;
        bool operator!=(const Transform &t) const;

        bool isIdentity() const;
        bool willSwapHandedness() const;
        const Matrix44 &getMatrix() const;
        const Matrix44 &getInverse() const;

        friend Transform inverse(const Transform &t);
        friend Transform transpose(const Transform &t);

    private:
        Matrix44 m;
        Matrix44 mInv;
    };

    inline bool Transform::operator==(const Transform &t) const {
        return t.m == m && t.mInv == mInv;
    }
    inline bool Transform::operator!=(const Transform &t) const {
        return t.m != m || t.mInv != mInv;
    }

    template <typename T>
    inline Point3<T> Transform::operator()(const Point3<T> &p) const {
        T px = p.x, py = p.y, pz = p.z;
        T x = m.data[0] * px + m.data[1] * py + m.data[2] * pz + m.data[3];
        T y = m.data[4] * px + m.data[5] * py + m.data[6] * pz + m.data[7];
        T z = m.data[8] * px + m.data[9] * py + m.data[10] * pz + m.data[11];
        T w = m.data[12] * px + m.data[13] * py + m.data[14] * pz + m.data[15];
        if (w == 1)
            return Point3<T>(x, y, z);
        else
            return Point3<T>(x, y, z) / w;
    }
    template <typename T>
    inline Vec3<T> Transform::operator()(const Vec3<T> &v, bool isNormal) const {
        T vx = v.x, vy = v.y, vz = v.z;

        if (isNormal) {
            return Vec3<T>(
                mInv.data[0] * vx + mInv.data[4] * vy + mInv.data[8] * vz,
                mInv.data[1] * vx + mInv.data[5] * vy + mInv.data[9] * vz,
                mInv.data[2] * vx + mInv.data[6] * vy + mInv.data[10] * vz);
        } else {
            return Vec3<T>(
                m.data[0] * vx + m.data[1] * vy + m.data[2] * vz,
                m.data[4] * vx + m.data[5] * vy + m.data[6] * vz,
                m.data[8] * vx + m.data[9] * vy + m.data[10] * vz);
        }
    }
    template <typename T>
    inline AABB<T> Transform::operator()(const AABB<T> &bb) const {
        const Transform &mat = *this;
        AABB<T> out(mat(bb.corner(0)));
        for (tkUInt i = 1; i < 8; ++i) {
            out = bbUnion(out, mat(bb.corner(i)));
        }
        return out;
    }
    inline Ray Transform::operator()(const Ray &r) const {
        tkPoint3f o = (*this)(r.o);
        tkVec3f d = (*this)(r.d);

        return Ray(o, d, r.tMax/*, r.time, r.medium*/);
    }

    inline Transform inverse(const Transform &t) {
        return Transform(t.mInv, t.m);
    }
    inline Transform transpose(const Transform &t) {
        return Transform(transpose(t.m), transpose(t.mInv));
    }

    inline Transform operator*(const Transform &t1, const Transform &t2) {
        return Transform(t1.getMatrix() * t2.getMatrix(),
                         t2.getInverse() * t1.getInverse());
    }

    // Affine transformations
    Transform translate(const tkVec3f &offset);
    Transform scale(tkFloat x, tkFloat y, tkFloat z);
    Transform rotate(const tkVec3f &axis, tkFloat theta);
    Transform rotateX(tkFloat theta);
    Transform rotateY(tkFloat theta);
    Transform rotateZ(tkFloat theta);

    // View transformation
    Transform lookAt(const tkPoint3f &eye, const tkPoint3f &at, const tkVec3f &up);

    // Projection transformation
    Transform orthographic(tkFloat near, tkFloat far);
    Transform perspective(tkFloat fovy, tkFloat near, tkFloat far);
} // namespace TK
