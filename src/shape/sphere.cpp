#include "sphere.hpp"

#include "core/ray.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    inline tkAABBf Sphere::objectBoundingBox() const {
        return tkAABBf(tkPoint3f(-radius), tkPoint3f(radius));
    }

    tkFloat Sphere::surfaceArea() const {
        return 4 * TK_PI * radius * radius;
    }

    bool Sphere::intersect(const Ray &r, tkFloat *tHit,
                           SurfaceInteraction *interaction) const {
        Ray oRay = objectToWorld->applyInverse(r);
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
        Ray oRay = objectToWorld->applyInverse(r);
        tkVec3f r0 = tkVec3f(oRay.o);
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

    SurfaceInteraction Sphere::sample(const Interaction &ref,
                                      const tkVec2f &samp, tkFloat *pdf) const {
        tkPoint3f center = (*objectToWorld)(tkPoint3f::zero);
        tkVec3f centerToRef = ref.p - center;
        tkVec3f z = normalize(centerToRef);
        tkVec3f x, y;
        coordinateSystem(z, &x, &y);
        SurfaceInteraction ret;

        tkFloat sqrDist = centerToRef.squaredMagnitude();
        tkFloat sqrRadius = radius * radius;

        // Check if ref is within sphere
        if (sqrDist <= sqrRadius) {
            tkVec3f n = radius * uniformSphereSample(samp[0], samp[1]);
            ret.n = (*objectToWorld)(n, true);
            ret.p = (*objectToWorld)(tkPoint3f(n));
            if (invertNormals)
                ret.n = -ret.n;
            tkVec3f dir = ref.p - ret.p;
            ret.wo = normalize(dir);

            // Similar to Shape::getPdf but skips intersection test
            tkFloat sampleSqrDist = dir.squaredMagnitude();
            if (sampleSqrDist == 0)
                *pdf = 0;
            else {
                tkFloat cosTheta = std::abs(dot(normalize(dir), ret.n));
                *pdf = sampleSqrDist / (cosTheta * surfaceArea());
            }
            return ret;
        }

        tkFloat cosSqrTheta = 1 - sqrRadius / sqrDist;
        tkFloat cosThetaMin = std::sqrt(clamp(cosSqrTheta, 0, 1));

        // Random sample cone
        tkFloat cosTheta = 1 + (cosThetaMin - 1) * samp[0];
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
        *pdf = uniformConePdf(cosThetaMin);
        return ret;
    }

    tkFloat Sphere::getPdf(const Interaction &ref, const tkVec3f &wi) const {
        tkPoint3f center = (*objectToWorld)(tkPoint3f::zero);
        tkFloat sqrRadius = radius * radius;
        tkFloat sqrDist = squaredDistance(ref.p, center);
        if (sqrDist <= sqrRadius)
            return Shape::getPdf(ref, wi);

        tkFloat cosSqrTheta = 1 - sqrRadius / sqrDist;
        tkFloat cosThetaMin = std::sqrt(clamp(cosSqrTheta, 0, 1));
        return uniformConePdf(cosThetaMin);
    }
} // namespspace TK