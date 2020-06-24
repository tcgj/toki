#include "sphere.hpp"

#include "geometry/ray.hpp"

namespace TK {
    inline tkAABBf Sphere::objectBoundingBox() const {
        return tkAABBf(tkPoint3f(-radius), tkPoint3f(radius));
    }

    bool Sphere::intersect(const Ray &r, tkFloat *tHit,
                           SurfaceInteraction *interaction) const {
        Ray oRay = (inverse(*objectToWorld))(r);
        tkVec3f r0 = tkVec3f(oRay.o);
        tkFloat a = dot(oRay.d, oRay.d);
        tkFloat b = 2 * dot(oRay.d, r0);
        tkFloat c = dot(r0, r0) - radius * radius;

        tkFloat t0, t1;
        if (!quadratic(a, b, c, &t0, &t1))
            return false;
        if (t1 < TK_EPSILON || t0 > r.tMax)
            return false;
        if (t0 < TK_EPSILON)
            t0 = t1;

        *tHit = t0;
        tkVec3f normal = normalize(tkVec3f(oRay(*tHit)));
        normal = (*objectToWorld)(normal, true);
        interaction->p = r(*tHit);
        interaction->n = invertNormals ? -normal : normal;
        interaction->wo = -r.d;
        interaction->shape = this;

        return true;
    }

    bool Sphere::hasIntersect(const Ray &r) const {
        Ray oRay = (inverse(*objectToWorld))(r);
        tkVec3f r0 = oRay.o - tkPoint3f::zero;
        tkFloat a = dot(oRay.d, oRay.d);
        tkFloat b = 2 * dot(oRay.d, r0);
        tkFloat c = dot(r0, r0) - radius * radius;

        tkFloat t0, t1;
        if (!quadratic(a, b, c, &t0, &t1))
            return false;
        if (t1 < TK_EPSILON || t0 > r.tMax)
            return false;

        return true;
    }
} // namespspace TK