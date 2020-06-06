#pragma once

#include "system/system.hpp"
#include "math/math.hpp"
#include "ray.hpp"

namespace TK {
    template <typename T>
    class AABB {
    public:
        AABB() {
            T minVal = std::numeric_limits<T>::lowest();
            T maxVal = std::numeric_limits<T>::max();
            minPt = Point3<T>(maxVal);
            maxPt = Point3<T>(minVal);
        }
        AABB(const Point2<T> &p) : minPt(p, 0), maxPt(p, 0) {}
        AABB(const Point2<T> &p1, const Point2<T> &p2)
            : minPt(std::min(p1.x, p2.x), std::min(p1.y, p2.y), 0),
              maxPt(std::max(p1.x, p2.x), std::max(p1.y, p2.y), 0) {}
        AABB(const Point3<T> &p) : minPt(p), maxPt(p) {}
        AABB(const Point3<T> &p1, const Point3<T> &p2)
            : minPt(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
              maxPt(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)) {}

        // Subscript operators
        const Point3<T> &operator[](tkInt i) const;
        Point3<T> operator[](tkInt i);

        // Equality
        bool operator==(const AABB<T> &b) const;
        bool operator!=(const AABB<T> &b) const;

        // Math
        Point3<T> corner(tkUInt cIndex) const;
        Vec3<T> diagonal() const;
        T surfaceArea() const;
        T area(tkInt axis1, tkInt axis2) const;
        T volume() const;
        tkUInt maxExtent() const;
        Point3<T> lerp(const tkPoint3f &t) const;
        Vec3<T> offset(const Point3<T> &p) const;
        void boundingSphere(Point3<T> *center, tkFloat *radius) const;

        // Ray-BB intersection test
        bool hasIntersect(const Ray &r) const;
        bool hasIntersect(const Ray &r, const tkVec3f &invD,
                          const tkInt dirNegative[3]) const;

        Point3<T> minPt, maxPt;
    };

    template <typename T>
    inline const Point3<T> &AABB<T>::operator[](tkInt i) const {
        tkAssert(i == 0 || i == 1);
        return i == 0 ? minPt : maxPt;
    }

    template <typename T>
    inline Point3<T> AABB<T>::operator[](tkInt i) {
        tkAssert(i == 0 || i == 1);
        return i == 0 ? minPt : maxPt;
    }

    template <typename T>
    inline bool AABB<T>::operator==(const AABB<T> &b) const {
        return b.minPt == minPt && b.maxPt == maxPt;
    }
    template <typename T>
    inline bool AABB<T>::operator!=(const AABB<T> &b) const {
        return b.minPt != minPt || b.maxPt != maxPt;
    };

    template <typename T>
    inline Point3<T> AABB<T>::corner(tkUInt cIndex) const {
        return Point3<T>((*this)[cIndex & 1].x,
                        (*this)[(cIndex & 2) ? 1 : 0].y,
                        (*this)[(cIndex & 4) ? 1 : 0].z);
    }

    template <typename T>
    inline Vec3<T> AABB<T>::diagonal() const {
        return maxPt - minPt;
    }
    template <typename T>
    inline T AABB<T>::surfaceArea() const {
        Vec3<T> d = diagonal();
        return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }
    template <typename T>
    inline T AABB<T>::area(tkInt axis1, tkInt axis2) const {
        Vec3<T> d = diagonal();
        return d[axis1] * d[axis2];
    }
    template <typename T>
    inline T AABB<T>::volume() const {
        Vec3<T> d = diagonal();
        return d.x * d.y * d.z;
    }
    template <typename T>
    inline tkUInt AABB<T>::maxExtent() const {
        Vec3<T> d = diagonal();
        if (d.x > d.y && d.x > d.z) {
            return 0;
        } else if (d.y > d.z) {
            return 1;
        } else {
            return 2;
        }
    }
    template <typename T>
    inline Point3<T> AABB<T>::lerp(const tkPoint3f &t) const {
        return Point3<T>(TK::lerp(minPt.x, maxPt.x, t.x),
                        TK::lerp(minPt.y, maxPt.y, t.y),
                        TK::lerp(minPt.z, maxPt.z, t.z));
    }
    template <typename T>
    inline Vec3<T> AABB<T>::offset(const Point3<T> &p) const {
        Vec3<T> o = p - minPt;
        if (maxPt.x > minPt.x) o.x /= maxPt.x - minPt.x;
        if (maxPt.y > minPt.y) o.y /= maxPt.y - minPt.y;
        if (maxPt.z > minPt.z) o.z /= maxPt.z - minPt.z;
        return o;
    }
    template <typename T>
    inline void AABB<T>::boundingSphere(Point3<T> *center, tkFloat *radius) const {
        *center = (minPt + maxPt) / 2;
        *radius = isInside(*center, *this) ? distance(*center, maxPt) : 0;
    }

    // Bounding box operations
    template <typename T>
    inline bool inside(const Point3<T> &p, const AABB<T> &b) {
        return (p.x >= b.minPt.x && p.x <= b.maxPt.x && p.y >= b.minPt.y &&
                p.y <= b.maxPt.y && p.z >= b.minPt.z && p.z <= b.maxPt.z);
    }
    template <typename T>
    inline bool insideExclusive(const Point3<T> &p, const AABB<T> &b) {
        return (p.x >= b.minPt.x && p.x < b.maxPt.x && p.y >= b.minPt.y &&
                p.y < b.maxPt.y && p.z >= b.minPt.z && p.z < b.maxPt.z);
    }
    template <typename T>
    inline bool overlap(const AABB<T> &b1, const AABB<T> &b2) {
        bool x = (b1.maxPt.x >= b2.minPt.x) && (b1.minPt.x <= b2.maxPt.x);
        bool y = (b1.maxPt.y >= b2.minPt.y) && (b1.minPt.y <= b2.maxPt.y);
        bool z = (b1.maxPt.z >= b2.minPt.z) && (b1.minPt.z <= b2.maxPt.z);
        return (x && y && z);
    }
    template <typename T, typename U>
    inline AABB<T> expand(const AABB<T> &b, U delta) {
        return AABB<T>(b.minPt - Vec3<T>(delta), b.maxPt + Vec3<T>(delta));
    }

    template <typename T>
    inline AABB<T> bbUnion(const AABB<T> &b1, const AABB<T> &b2) {
        return AABB<T>(Point3<T>(std::min(b1.minPt.x, b2.minPt.x),
                                std::min(b1.minPt.y, b2.minPt.y),
                                std::min(b1.minPt.z, b2.minPt.z)),
                    Point3<T>(std::max(b1.maxPt.x, b2.maxPt.x),
                                std::max(b1.maxPt.y, b2.maxPt.y),
                                std::max(b1.maxPt.z, b2.maxPt.z)));
    }
    template <typename T>
    inline AABB<T> bbUnion(const AABB<T> &b, const Point3<T> &p) {
        return AABB<T>(Point3<T>(std::min(b.minPt.x, p.x),
                                std::min(b.minPt.y, p.y),
                                std::min(b.minPt.z, p.z)),
                    Point3<T>(std::max(b.maxPt.x, p.x),
                                std::max(b.maxPt.y, p.y),
                                std::max(b.maxPt.z, p.z)));
    }
    template <typename T>
    inline AABB<T> bbIntersect(const AABB<T> &b1, const AABB<T> &b2) {
        return AABB<T>(Point3<T>(std::max(b1.minPt.x, b2.minPt.x),
                                std::max(b1.minPt.y, b2.minPt.y),
                                std::max(b1.minPt.z, b2.minPt.z)),
                    Point3<T>(std::min(b1.maxPt.x, b2.maxPt.x),
                                std::min(b1.maxPt.y, b2.maxPt.y),
                                std::min(b1.maxPt.z, b2.maxPt.z)));
    }

    template <typename T>
    inline bool AABB<T>::hasIntersect(const Ray &r) const {
        tkFloat tmin = 0, tmax = r.tMax;
        for (tkUInt i = 0; i < 3; ++i) {
            // component-wise computation
            // p = o + t*d -----> t = (p - o) * 1/d
            tkFloat invD = 1.0 / r.d[i];
            tkFloat t0 = (minPt[i] - r.o[i]) * invD;
            tkFloat t1 = (minPt[i] - r.o[i]) * invD;
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
    inline bool AABB<T>::hasIntersect(const Ray &r, const tkVec3f &invD,
                               const tkInt dirNegative[3]) const {
        const AABB<T> &bb = *this;
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
} // namespace TK
