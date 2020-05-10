#pragma once

#include "math.hpp"
#include "matrix44.hpp"
#include "ray.hpp"

namespace TK {
    class Transform {
    public:
        Transform() {}
        Transform(const tkFloat d[16]) {
            m = Matrix44(d[0], d[1], d[2], d[3],
                         d[4], d[5], d[6],  d[7],
                         d[8], d[9],  d[10], d[11],
                         d[12],  d[13], d[14], d[15]);
            mInv = inverse(m);
        }
        Transform(const Matrix44& m) : m(m), mInv(inverse(m)) {}
        Transform(const Matrix44& m, const Matrix44& mInv) : m(m), mInv(mInv) {}

        // Transform Application
        template <typename T> Point3<T> operator()(const Point3<T>& p) const;
        template <typename T> Vec3<T> operator()(const Vec3<T>& v, bool isNormal = false) const;
        template <typename T> AABB<T> operator()(const AABB<T>& bb) const;
        Ray operator()(const Ray& r) const;
        // (With FPE handling)
        template <typename T> Point3<T> operator()(const Point3<T>& p, Vec3<T>* absError) const;

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

    bool Transform::operator==(const Transform& t) const {
        return t.m == m && t.mInv == mInv;
    }
    bool Transform::operator!=(const Transform& t) const {
        return t.m != m || t.mInv != mInv;
    }

    Transform inverse(const Transform& t) {
        return Transform(t.mInv, t.m);
    }
    Transform transpose(const Transform& t) {
        return Transform(transpose(t.m), transpose(t.mInv));
    }

    Transform operator*(const Transform& t1, const Transform& t2) {
        return Transform(t1.getMatrix() * t2.getMatrix(),
                         t2.getInverse() * t1.getInverse());
    }

    // Affine transformations
    Transform translate(const tkVec3f& offset);
    Transform scale(tkFloat x, tkFloat y, tkFloat z);
    Transform rotate(const tkVec3f& axis, tkFloat theta);
    Transform rotateX(tkFloat theta);
    Transform rotateY(tkFloat theta);
    Transform rotateZ(tkFloat theta);

    // View transformation
    Transform lookAt(const tkPoint3f& eye, const tkPoint3f& at, const tkVec3f& up);
} // namespace TK
