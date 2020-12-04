#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"
#include "ray.hpp"

namespace TK {
    class Transform {
    public:
        Transform() = default;
        Transform(const tkFloat d[16]) {
            m = Matrix44(d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9], d[10], d[11], d[12],
                         d[13], d[14], d[15]);
            mInv = inverse(m);
        }
        Transform(const Matrix44& m) : m(m), mInv(inverse(m)) {}
        Transform(const Matrix44& m, const Matrix44& mInv) : m(m), mInv(mInv) {}

        // Transform Application
        template <typename T>
        Point3<T> operator()(const Point3<T>& p) const;
        template <typename T>
        Vec3<T> operator()(const Vec3<T>& v, bool isNormal = false) const;
        template <typename T>
        AABB<T> operator()(const AABB<T>& bb) const;
        Ray operator()(const Ray& r) const;

        template <typename T>
        Point3<T> applyInverse(const Point3<T>& p) const;
        template <typename T>
        Vec3<T> applyInverse(const Vec3<T>& v, bool isNormal = false) const;
        template <typename T>
        AABB<T> applyInverse(const AABB<T>& bb) const;
        Ray applyInverse(const Ray& r) const;

        // Equality
        bool operator==(const Transform& t) const;
        bool operator!=(const Transform& t) const;

        bool isIdentity() const;
        bool willSwapHandedness() const;
        const Matrix44& getMatrix() const;
        const Matrix44& getInverse() const;

        friend Transform inverse(const Transform& t);
        friend Transform transpose(const Transform& t);

    private:
        Matrix44 m;
        Matrix44 mInv;
    };

    inline bool Transform::operator==(const Transform& t) const {
        return t.m == m && t.mInv == mInv;
    }
    inline bool Transform::operator!=(const Transform& t) const {
        return t.m != m || t.mInv != mInv;
    }

    template <typename T>
    inline Point3<T> Transform::operator()(const Point3<T>& p) const {
        T px = p.x, py = p.y, pz = p.z;
        T x = m.entries[0] * px + m.entries[1] * py + m.entries[2] * pz + m.entries[3];
        T y = m.entries[4] * px + m.entries[5] * py + m.entries[6] * pz + m.entries[7];
        T z = m.entries[8] * px + m.entries[9] * py + m.entries[10] * pz + m.entries[11];
        T w = m.entries[12] * px + m.entries[13] * py + m.entries[14] * pz + m.entries[15];
        if (w == 1)
            return Point3<T>(x, y, z);
        else
            return Point3<T>(x, y, z) / w;
    }
    template <typename T>
    inline Vec3<T> Transform::operator()(const Vec3<T>& v, bool isNormal) const {
        T vx = v.x, vy = v.y, vz = v.z;

        if (isNormal) {
            return Vec3<T>(mInv.entries[0] * vx + mInv.entries[4] * vy + mInv.entries[8] * vz,
                           mInv.entries[1] * vx + mInv.entries[5] * vy + mInv.entries[9] * vz,
                           mInv.entries[2] * vx + mInv.entries[6] * vy + mInv.entries[10] * vz);
        } else {
            return Vec3<T>(m.entries[0] * vx + m.entries[1] * vy + m.entries[2] * vz,
                           m.entries[4] * vx + m.entries[5] * vy + m.entries[6] * vz,
                           m.entries[8] * vx + m.entries[9] * vy + m.entries[10] * vz);
        }
    }
    template <typename T>
    inline AABB<T> Transform::operator()(const AABB<T>& bb) const {
        const Transform& mat = *this;
        AABB<T> out(mat(bb.corner(0)));
        for (int i = 1; i < 8; ++i) {
            out = bbUnion(out, mat(bb.corner(i)));
        }
        return out;
    }
    inline Ray Transform::operator()(const Ray& r) const {
        Point3f o = (*this)(r.o);
        Vec3f d = (*this)(r.d);

        return Ray(o, d, r.tMax /*, r.time, r.medium*/);
    }

    template <typename T>
    inline Point3<T> Transform::applyInverse(const Point3<T>& p) const {
        T px = p.x, py = p.y, pz = p.z;
        T x = mInv.entries[0] * px + mInv.entries[1] * py + mInv.entries[2] * pz + mInv.entries[3];
        T y = mInv.entries[4] * px + mInv.entries[5] * py + mInv.entries[6] * pz + mInv.entries[7];
        T z = mInv.entries[8] * px + mInv.entries[9] * py + mInv.entries[10] * pz + mInv.entries[11];
        T w = mInv.entries[12] * px + mInv.entries[13] * py + mInv.entries[14] * pz + mInv.entries[15];
        if (w == 1)
            return Point3<T>(x, y, z);
        else
            return Point3<T>(x, y, z) / w;
    }
    template <typename T>
    inline Vec3<T> Transform::applyInverse(const Vec3<T>& v, bool isNormal) const {
        T vx = v.x, vy = v.y, vz = v.z;

        if (isNormal) {
            return Vec3<T>(m.entries[0] * vx + m.entries[4] * vy + m.entries[8] * vz,
                           m.entries[1] * vx + m.entries[5] * vy + m.entries[9] * vz,
                           m.entries[2] * vx + m.entries[6] * vy + m.entries[10] * vz);
        } else {
            return Vec3<T>(mInv.entries[0] * vx + mInv.entries[1] * vy + mInv.entries[2] * vz,
                           mInv.entries[4] * vx + mInv.entries[5] * vy + mInv.entries[6] * vz,
                           mInv.entries[8] * vx + mInv.entries[9] * vy + mInv.entries[10] * vz);
        }
    }
    template <typename T>
    inline AABB<T> Transform::applyInverse(const AABB<T>& bb) const {
        AABB<T> out(this->applyInverse(bb.corner(0)));
        for (int i = 1; i < 8; ++i) {
            out = bbUnion(out, this->applyInverse(bb.corner(i)));
        }
        return out;
    }
    inline Ray Transform::applyInverse(const Ray& r) const {
        Point3f o = this->applyInverse(r.o);
        Vec3f d = this->applyInverse(r.d);

        return Ray(o, d, r.tMax /*, r.time, r.medium*/);
    }

    inline Transform inverse(const Transform& t) {
        return Transform(t.mInv, t.m);
    }
    inline Transform transpose(const Transform& t) {
        return Transform(transpose(t.m), transpose(t.mInv));
    }

    inline Transform operator*(const Transform& t1, const Transform& t2) {
        return Transform(t1.getMatrix() * t2.getMatrix(), t2.getInverse() * t1.getInverse());
    }

    // Affine transformations
    Transform translate(const Vec3f& offset);
    Transform scale(tkFloat x, tkFloat y, tkFloat z);
    Transform rotate(const Vec3f& axis, tkFloat theta);
    Transform rotateX(tkFloat theta);
    Transform rotateY(tkFloat theta);
    Transform rotateZ(tkFloat theta);

    // View transformation
    Transform lookAt(const Point3f& eye, const Point3f& at, const Vec3f& up);

    // Projection transformation
    Transform orthographic(tkFloat near, tkFloat far);
    Transform perspective(tkFloat fovy, tkFloat aspect, tkFloat near, tkFloat far);
}  // namespace TK
