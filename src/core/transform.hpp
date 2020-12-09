#pragma once

#include "system/toki.hpp"
#include "math/matrix44.hpp"
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

        std::string toString() const;

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
        T x = m.m_Entries[0] * px + m.m_Entries[1] * py + m.m_Entries[2] * pz + m.m_Entries[3];
        T y = m.m_Entries[4] * px + m.m_Entries[5] * py + m.m_Entries[6] * pz + m.m_Entries[7];
        T z = m.m_Entries[8] * px + m.m_Entries[9] * py + m.m_Entries[10] * pz + m.m_Entries[11];
        T w = m.m_Entries[12] * px + m.m_Entries[13] * py + m.m_Entries[14] * pz + m.m_Entries[15];
        if (w == 1)
            return Point3<T>(x, y, z);
        else
            return Point3<T>(x, y, z) / w;
    }

    template <typename T>
    inline Vec3<T> Transform::operator()(const Vec3<T>& v, bool isNormal) const {
        T vx = v.x, vy = v.y, vz = v.z;

        if (isNormal) {
            return Vec3<T>(mInv.m_Entries[0] * vx + mInv.m_Entries[4] * vy + mInv.m_Entries[8] * vz,
                           mInv.m_Entries[1] * vx + mInv.m_Entries[5] * vy + mInv.m_Entries[9] * vz,
                           mInv.m_Entries[2] * vx + mInv.m_Entries[6] * vy + mInv.m_Entries[10] * vz);
        } else {
            return Vec3<T>(m.m_Entries[0] * vx + m.m_Entries[1] * vy + m.m_Entries[2] * vz,
                           m.m_Entries[4] * vx + m.m_Entries[5] * vy + m.m_Entries[6] * vz,
                           m.m_Entries[8] * vx + m.m_Entries[9] * vy + m.m_Entries[10] * vz);
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
        T x = mInv.m_Entries[0] * px + mInv.m_Entries[1] * py + mInv.m_Entries[2] * pz + mInv.m_Entries[3];
        T y = mInv.m_Entries[4] * px + mInv.m_Entries[5] * py + mInv.m_Entries[6] * pz + mInv.m_Entries[7];
        T z = mInv.m_Entries[8] * px + mInv.m_Entries[9] * py + mInv.m_Entries[10] * pz + mInv.m_Entries[11];
        T w =
            mInv.m_Entries[12] * px + mInv.m_Entries[13] * py + mInv.m_Entries[14] * pz + mInv.m_Entries[15];
        if (w == 1)
            return Point3<T>(x, y, z);
        else
            return Point3<T>(x, y, z) / w;
    }

    template <typename T>
    inline Vec3<T> Transform::applyInverse(const Vec3<T>& v, bool isNormal) const {
        T vx = v.x, vy = v.y, vz = v.z;

        if (isNormal) {
            return Vec3<T>(m.m_Entries[0] * vx + m.m_Entries[4] * vy + m.m_Entries[8] * vz,
                           m.m_Entries[1] * vx + m.m_Entries[5] * vy + m.m_Entries[9] * vz,
                           m.m_Entries[2] * vx + m.m_Entries[6] * vy + m.m_Entries[10] * vz);
        } else {
            return Vec3<T>(mInv.m_Entries[0] * vx + mInv.m_Entries[1] * vy + mInv.m_Entries[2] * vz,
                           mInv.m_Entries[4] * vx + mInv.m_Entries[5] * vy + mInv.m_Entries[6] * vz,
                           mInv.m_Entries[8] * vx + mInv.m_Entries[9] * vy + mInv.m_Entries[10] * vz);
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

    inline std::string Transform::toString() const {
        return m.toString();
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

    std::istream& operator>>(std::istream& is, const Transform& t);
    std::ostream& operator<<(std::ostream& os, const Transform& t);

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
