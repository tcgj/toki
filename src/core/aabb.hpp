#pragma once

#include "system/toki.hpp"
#include "ray.hpp"

namespace TK {
    template <typename T>
    class AABB {
    public:
        AABB() {
            T minVal = std::numeric_limits<T>::lowest();
            T maxVal = std::numeric_limits<T>::max();
            m_Min = Point3<T>(maxVal);
            m_Max = Point3<T>(minVal);
        }

        AABB(const Point2<T>& p) : m_Min(p, 0), m_Max(p, 0) {}

        AABB(const Point2<T>& p1, const Point2<T>& p2)
            : m_Min(std::min(p1.x, p2.x), std::min(p1.y, p2.y), 0),
              m_Max(std::max(p1.x, p2.x), std::max(p1.y, p2.y), 0) {}

        AABB(const Point3<T>& p) : m_Min(p), m_Max(p) {}

        AABB(const Point3<T>& p1, const Point3<T>& p2)
            : m_Min(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
              m_Max(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)) {}

        // Subscript operators
        const Point3<T>& operator[](int i) const;

        Point3<T> operator[](int i);

        // Equality
        bool operator==(const AABB<T>& b) const;

        bool operator!=(const AABB<T>& b) const;

        // Math
        Point3<T> corner(int cIndex) const;

        Point3<T> center() const;

        Vec3<T> diagonal() const;

        T surfaceArea() const;

        T area(int axis1, int axis2) const;

        T volume() const;

        int maxExtent() const;

        Point3<T> lerp(const Point3f& t) const;

        Vec3<T> offset(const Point3<T>& p) const;

        void boundingSphere(Point3<T>* center, tkFloat* radius) const;

        // Ray-BB intersection test
        bool hasIntersect(const Ray& r) const;

        bool hasIntersect(const Ray& r, const Vec3f& invD, const int dirNegative[3]) const;

        Point3<T> m_Min;
        Point3<T> m_Max;
    };

    template <typename T>
    inline const Point3<T>& AABB<T>::operator[](int i) const {
        return i == 0 ? m_Min : m_Max;
    }

    template <typename T>
    inline Point3<T> AABB<T>::operator[](int i) {
        return i == 0 ? m_Min : m_Max;
    }

    template <typename T>
    inline bool AABB<T>::operator==(const AABB<T>& b) const {
        return b.m_Min == m_Min && b.m_Max == m_Max;
    }

    template <typename T>
    inline bool AABB<T>::operator!=(const AABB<T>& b) const {
        return b.m_Min != m_Min || b.m_Max != m_Max;
    };

    template <typename T>
    inline Point3<T> AABB<T>::corner(int cIndex) const {
        return Point3<T>((*this)[cIndex & 1].x, (*this)[(cIndex & 2) ? 1 : 0].y,
                         (*this)[(cIndex & 4) ? 1 : 0].z);
    }

    template <typename T>
    inline Point3<T> AABB<T>::center() const {
        return 0.5 * (m_Min + m_Max);
    }

    template <typename T>
    inline Vec3<T> AABB<T>::diagonal() const {
        return m_Max - m_Min;
    }

    template <typename T>
    inline T AABB<T>::surfaceArea() const {
        Vec3<T> d = diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    template <typename T>
    inline T AABB<T>::area(int axis1, int axis2) const {
        Vec3<T> d = diagonal();
        return d[axis1] * d[axis2];
    }

    template <typename T>
    inline T AABB<T>::volume() const {
        Vec3<T> d = diagonal();
        return d.x * d.y * d.z;
    }

    template <typename T>
    inline int AABB<T>::maxExtent() const {
        Vec3<T> d = diagonal();
        if (d.x > d.y && d.x > d.z)
            return 0;
        else if (d.y > d.z)
            return 1;
        else
            return 2;
    }

    template <typename T>
    inline Point3<T> AABB<T>::lerp(const Point3f& t) const {
        return Point3<T>(TK::lerp(m_Min.x, m_Max.x, t.x), TK::lerp(m_Min.y, m_Max.y, t.y),
                         TK::lerp(m_Min.z, m_Max.z, t.z));
    }

    template <typename T>
    inline Vec3<T> AABB<T>::offset(const Point3<T>& p) const {
        Vec3<T> o = p - m_Min;
        if (m_Max.x > m_Min.x)
            o.x /= m_Max.x - m_Min.x;
        if (m_Max.y > m_Min.y)
            o.y /= m_Max.y - m_Min.y;
        if (m_Max.z > m_Min.z)
            o.z /= m_Max.z - m_Min.z;
        return o;
    }

    // Bounding box operations
    template <typename T>
    inline bool isInside(const Point3<T>& p, const AABB<T>& b) {
        return (p.x >= b.m_Min.x && p.x <= b.m_Max.x && p.y >= b.m_Min.y && p.y <= b.m_Max.y &&
                p.z >= b.m_Min.z && p.z <= b.m_Max.z);
    }

    template <typename T>
    inline bool isInsideExclusive(const Point3<T>& p, const AABB<T>& b) {
        return (p.x >= b.m_Min.x && p.x < b.m_Max.x && p.y >= b.m_Min.y && p.y < b.m_Max.y &&
                p.z >= b.m_Min.z && p.z < b.m_Max.z);
    }

    template <typename T>
    inline bool overlap(const AABB<T>& b1, const AABB<T>& b2) {
        bool x = (b1.m_Max.x >= b2.m_Min.x) && (b1.m_Min.x <= b2.m_Max.x);
        bool y = (b1.m_Max.y >= b2.m_Min.y) && (b1.m_Min.y <= b2.m_Max.y);
        bool z = (b1.m_Max.z >= b2.m_Min.z) && (b1.m_Min.z <= b2.m_Max.z);
        return (x && y && z);
    }

    template <typename T, typename U>
    inline AABB<T> expand(const AABB<T>& b, U delta) {
        return AABB<T>(b.m_Min - Vec3<T>(delta), b.m_Max + Vec3<T>(delta));
    }

    template <typename T>
    inline AABB<T> bbUnion(const AABB<T>& b1, const AABB<T>& b2) {
        return AABB<T>(Point3<T>(std::min(b1.m_Min.x, b2.m_Min.x), std::min(b1.m_Min.y, b2.m_Min.y),
                                 std::min(b1.m_Min.z, b2.m_Min.z)),
                       Point3<T>(std::max(b1.m_Max.x, b2.m_Max.x), std::max(b1.m_Max.y, b2.m_Max.y),
                                 std::max(b1.m_Max.z, b2.m_Max.z)));
    }

    template <typename T>
    inline AABB<T> bbUnion(const AABB<T>& b, const Point3<T>& p) {
        return AABB<T>(
            Point3<T>(std::min(b.m_Min.x, p.x), std::min(b.m_Min.y, p.y), std::min(b.m_Min.z, p.z)),
            Point3<T>(std::max(b.m_Max.x, p.x), std::max(b.m_Max.y, p.y), std::max(b.m_Max.z, p.z)));
    }

    template <typename T>
    inline AABB<T> bbIntersect(const AABB<T>& b1, const AABB<T>& b2) {
        return AABB<T>(Point3<T>(std::max(b1.m_Min.x, b2.m_Min.x), std::max(b1.m_Min.y, b2.m_Min.y),
                                 std::max(b1.m_Min.z, b2.m_Min.z)),
                       Point3<T>(std::min(b1.m_Max.x, b2.m_Max.x), std::min(b1.m_Max.y, b2.m_Max.y),
                                 std::min(b1.m_Max.z, b2.m_Max.z)));
    }

    template <typename T>
    inline void AABB<T>::boundingSphere(Point3<T>* center, tkFloat* radius) const {
        *center = (m_Min + m_Max) / 2;
        *radius = isInside(*center, *this) ? distance(*center, m_Max) : 0;
    }

    template <typename T>
    inline bool AABB<T>::hasIntersect(const Ray& r) const {
        tkFloat tmin = 0, tmax = r.tMax;
        for (int i = 0; i < 3; ++i) {
            // component-wise computation
            // p = o + t*d -----> t = (p - o) * 1/d
            tkFloat invD = 1.0 / r.d[i];
            tkFloat t0 = (m_Min[i] - r.o[i]) * invD;
            tkFloat t1 = (m_Min[i] - r.o[i]) * invD;
            if (t0 > t1)
                std::swap(t0, t1);
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;

            if (tmin > tmax)
                return false;
        }
        return true;
    }

    template <typename T>
    inline bool AABB<T>::hasIntersect(const Ray& r, const Vec3f& invD, const int dirNegative[3]) const {
        const AABB<T>& bb = *this;
        tkFloat tMin = (bb[dirNegative[0]].x - r.o.x) * invD.x;
        tkFloat tMax = (bb[1 - dirNegative[0]].x - r.o.x) * invD.x;
        tkFloat tyMin = (bb[dirNegative[1]].y - r.o.y) * invD.y;
        tkFloat tyMax = (bb[1 - dirNegative[1]].y - r.o.y) * invD.y;
        if (tMin > tyMax || tyMin > tMax)
            return false;
        if (tyMin > tMin)
            tMin = tyMin;
        if (tyMax < tMax)
            tMax = tyMax;
        tkFloat tzMin = (bb[dirNegative[2]].z - r.o.z) * invD.z;
        tkFloat tzMax = (bb[1 - dirNegative[2]].z - r.o.z) * invD.z;
        if (tMin > tzMax || tzMin > tMax)
            return false;
        if (tzMin > tMin)
            tMin = tzMin;
        if (tzMax < tMax)
            tMax = tzMax;
        return tMin < r.tMax && tMax > 0;
    }
}  // namespace TK
