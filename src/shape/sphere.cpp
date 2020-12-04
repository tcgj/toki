#include "sphere.hpp"

#include "core/ray.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    AABBf Sphere::objectBoundingBox() const {
        return AABBf(Point3f(-radius), Point3f(radius));
    }

    tkFloat Sphere::surfaceArea() const {
        return 4 * TK_PI * radius * radius;
    }

    bool Sphere::intersect(const Ray& r, tkFloat* tHit, SurfaceInteraction* interaction) const {
        Ray oRay = objectToWorld->applyInverse(r);
        Vec3f r0 = Vec3f(oRay.o);
        tkFloat a = dot(oRay.d, oRay.d);
        tkFloat b = 2 * dot(oRay.d, r0);
        tkFloat c = dot(r0, r0) - radius * radius;

        tkFloat t0;
        tkFloat t1;
        if (!quadratic(a, b, c, &t0, &t1))
            return false;
        if (t1 < TK_EPSILON || t0 > r.tMax)
            return false;
        if (t0 < TK_EPSILON)
            t0 = t1;

        *tHit = t0;
        Vec3f normal = Vec3f(oRay(*tHit));
        Vec3f tangent = cross(Vec3f(0, 0, 1), normal);
        normal = (*objectToWorld)(normal, true);
        tangent = (*objectToWorld)(tangent);
        interaction->p = r(*tHit);
        interaction->n = normalize(invertNormals ? -normal : normal);
        interaction->dpdu = normalize(tangent);
        interaction->wo = -r.d;
        interaction->shape = this;
        return true;
    }

    bool Sphere::hasIntersect(const Ray& r) const {
        Ray oRay = objectToWorld->applyInverse(r);
        Vec3f r0 = Vec3f(oRay.o);
        tkFloat a = dot(oRay.d, oRay.d);
        tkFloat b = 2 * dot(oRay.d, r0);
        tkFloat c = dot(r0, r0) - radius * radius;

        tkFloat t0;
        tkFloat t1;
        if (!quadratic(a, b, c, &t0, &t1))
            return false;
        if (t1 < TK_EPSILON || t0 > r.tMax)
            return false;

        return true;
    }

    SurfaceInteraction Sphere::sample(const Interaction& ref, const Vec2f& samp, tkFloat* pdf) const {
        Point3f center = (*objectToWorld)(Point3f::zero);
        Vec3f centerToRef = ref.p - center;
        Vec3f z = normalize(centerToRef);
        Vec3f x;
        Vec3f y;
        coordinateSystem(z, &x, &y);
        SurfaceInteraction ret;

        tkFloat sqrDist = centerToRef.squaredMagnitude();
        tkFloat sqrRadius = radius * radius;

        // Check if ref is within sphere
        if (sqrDist <= sqrRadius) {
            Vec3f n = radius * uniformSphereSample(samp[0], samp[1]);
            ret.n = (*objectToWorld)(n, true);
            ret.p = (*objectToWorld)(Point3f(n));
            if (invertNormals)
                ret.n = -ret.n;
            ret.wo = normalize(ref.p - ret.p);
            *pdf = Shape::getPdf(ref, ret);
            return ret;
        }

        tkFloat cosSqrTheta = 1 - sqrRadius / sqrDist;
        tkFloat cosMaxTheta = std::sqrt(clamp(cosSqrTheta, 0, 1));

        // Random sample cone
        tkFloat cosTheta = 1 + (cosMaxTheta - 1) * samp[0];
        tkFloat sinSqrTheta = 1 - cosTheta * cosTheta;
        tkFloat phi = 2 * TK_PI * samp[1];

        tkFloat dist = std::sqrt(sqrDist);
        tkFloat sampleDist = dist * cosTheta - std::sqrt(sqrRadius - sqrDist * sinSqrTheta);

        // Find sample point perp-dist from z-axis(sinA) and z-value(cosA) in sphere frame
        tkFloat cosA = (sqrDist + sqrRadius - sampleDist * sampleDist) / (2 * dist * radius);
        tkFloat sinA = std::sqrt(clamp(1 - cosA * cosA, 0, 1));

        ret.n = polarToVec3(sinA, cosA, phi, x, y, z);
        ret.p = center + ret.n * radius;
        if (invertNormals)
            ret.n = -ret.n;
        ret.wo = normalize(ref.p - ret.p);
        *pdf = uniformConePdf(cosMaxTheta);
        return ret;
    }

    tkFloat Sphere::getPdf(const Interaction& ref, const Vec3f& wi) const {
        Point3f center = (*objectToWorld)(Point3f::zero);
        tkFloat sqrRadius = radius * radius;
        tkFloat sqrDist = squaredDistance(ref.p, center);
        if (sqrDist <= sqrRadius)
            return Shape::getPdf(ref, wi);

        tkFloat cosSqrTheta = 1 - sqrRadius / sqrDist;
        tkFloat cosMaxTheta = std::sqrt(clamp(cosSqrTheta, 0, 1));
        return uniformConePdf(cosMaxTheta);
    }
}  // namespace TK