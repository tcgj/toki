#include "aabb.hpp"

namespace TK {
    template <typename T>
    AABB<T> bbUnion(const AABB<T> &b1, const AABB<T> &b2) {
        return AABB<T>(Point3<T>(std::min(b1.minPt.x, b2.minPt.x),
                                std::min(b1.minPt.y, b2.minPt.y),
                                std::min(b1.minPt.z, b2.minPt.z)),
                    Point3<T>(std::max(b1.maxPt.x, b2.maxPt.x),
                                std::max(b1.maxPt.y, b2.maxPt.y),
                                std::max(b1.maxPt.z, b2.maxPt.z)));
    }
    template <typename T>
    AABB<T> bbUnion(const AABB<T> &b, const Point3<T> &p) {
        return AABB<T>(Point3<T>(std::min(b.minPt.x, p.x),
                                std::min(b.minPt.y, p.y),
                                std::min(b.minPt.z, p.z)),
                    Point3<T>(std::max(b.maxPt.x, p.x),
                                std::max(b.maxPt.y, p.y),
                                std::max(b.maxPt.z, p.z)));
    }
    template <typename T>
    AABB<T> bbIntersect(const AABB<T> &b1, const AABB<T> &b2) {
        return AABB<T>(Point3<T>(std::max(b1.minPt.x, b2.minPt.x),
                                std::max(b1.minPt.y, b2.minPt.y),
                                std::max(b1.minPt.z, b2.minPt.z)),
                    Point3<T>(std::min(b1.maxPt.x, b2.maxPt.x),
                                std::min(b1.maxPt.y, b2.maxPt.y),
                                std::min(b1.maxPt.z, b2.maxPt.z)));
    }

    template <typename T>
    bool hasIntersect(const AABB<T> &b, const Ray &r) {
        tkFloat tmin = 0, tmax = r.tMax;
        for (tkInt i = 0; i < 3; ++i) {
            // component-wise computation
            // p = o + t*d -----> t = (p - o) * 1/d
            tkFloat invD = (tkFloat)1 / r.d[i];
            tkFloat t0 = (b.minPt[i] - r.o[i]) * invD;
            tkFloat t1 = (b.minPt[i] - r.o[i]) * invD;
            if (t0 > t1)
                std::swap(t0, t1);
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;

            if (tmin > tmax)
                return false;
        }
        return true;
    }
} // namespace TK