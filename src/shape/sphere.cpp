#include "sphere.hpp"

#include "core/ray.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    AABBf Sphere::objectBoundingBox() const {
        return AABBf(Point3f(-m_Radius), Point3f(m_Radius));
    }

    tkFloat Sphere::surfaceArea() const {
        return 4 * TK_PI * m_Radius * m_Radius;
    }

    bool Sphere::intersect(const Ray& r, tkFloat& out_tHit, SurfaceInteraction& out_its) const {
        Ray oRay = m_ObjectToWorld->applyInverse(r);
        Vec3f r0 = Vec3f(oRay.o);
        tkFloat a = dot(oRay.d, oRay.d);
        tkFloat b = 2 * dot(oRay.d, r0);
        tkFloat c = std::fma(-m_Radius, m_Radius, dot(r0, r0));

        tkFloat t0;
        tkFloat t1;
        if (!quadratic(a, b, c, &t0, &t1))
            return false;
        if (t1 < TK_EPSILON || t0 > r.tMax)
            return false;

        if (t0 < TK_EPSILON)
            t0 = t1;
        out_tHit = t0;

        Vec3f normal(oRay(out_tHit));
        Vec3f tangent = cross(Vec3f(0, 0, 1), normal);
        normal = normalize((*m_ObjectToWorld)(m_InvertNormals ? -normal : normal, true));
        tangent = normalize((*m_ObjectToWorld)(tangent));
        out_its = SurfaceInteraction(r(out_tHit), normal, tangent, Vec3f::zero, normalize(-r.d), this);
        return true;
    }

    bool Sphere::hasIntersect(const Ray& r) const {
        Ray oRay = m_ObjectToWorld->applyInverse(r);
        Vec3f r0 = Vec3f(oRay.o);
        tkFloat a = dot(oRay.d, oRay.d);
        tkFloat b = 2 * dot(oRay.d, r0);
        tkFloat c = std::fma(-m_Radius, m_Radius, dot(r0, r0));

        tkFloat t0;
        tkFloat t1;
        if (!quadratic(a, b, c, &t0, &t1))
            return false;
        if (t1 < TK_EPSILON || t0 > r.tMax)
            return false;

        return true;
    }

    SurfaceInteraction Sphere::sample(const Interaction& ref, const Vec2f& u, tkFloat& out_pdf) const {
        Point3f center = (*m_ObjectToWorld)(Point3f::zero);
        Vec3f centerToRef = ref.p - center;
        Vec3f z = normalize(centerToRef);
        Vec3f x;
        Vec3f y;
        coordinateSystem(z, &x, &y);
        SurfaceInteraction ret;

        tkFloat sqrDist = centerToRef.squaredMagnitude();
        tkFloat sqrRadius = m_Radius * m_Radius;

        // Check if ref is within sphere
        if (sqrDist <= sqrRadius) {
            Vec3f n = m_Radius * uniformSphereSample(u[0], u[1]);
            ret.n = (*m_ObjectToWorld)(n, true);
            ret.p = (*m_ObjectToWorld)(Point3f(n));
            if (m_InvertNormals)
                ret.n = -ret.n;
            ret.wo = normalize(ref.p - ret.p);
            out_pdf = Shape::getPdf(ref, ret);
            return ret;
        }

        tkFloat cosSqrTheta = 1 - sqrRadius / sqrDist;
        tkFloat cosMaxTheta = std::sqrt(clamp(cosSqrTheta, 0, 1));

        // Random sample cone
        tkFloat cosTheta = 1 + (cosMaxTheta - 1) * u[0];
        tkFloat sinSqrTheta = 1 - cosTheta * cosTheta;
        tkFloat phi = 2 * TK_PI * u[1];

        tkFloat dist = std::sqrt(sqrDist);
        tkFloat sampleDist = dist * cosTheta - std::sqrt(sqrRadius - sqrDist * sinSqrTheta);

        // Find sample point perp-dist from z-axis(sinA) and z-value(cosA) in sphere frame
        tkFloat cosA = (sqrDist + sqrRadius - sampleDist * sampleDist) / (2 * dist * m_Radius);
        tkFloat sinA = std::sqrt(clamp(1 - cosA * cosA, 0, 1));

        ret.n = polarToVec3(sinA, cosA, phi, x, y, z);
        ret.p = center + ret.n * m_Radius;
        if (m_InvertNormals)
            ret.n = -ret.n;
        ret.wo = normalize(ref.p - ret.p);
        out_pdf = uniformConePdf(cosMaxTheta);
        return ret;
    }

    tkFloat Sphere::getPdf(const Interaction& ref, const Vec3f& wi) const {
        Point3f center = (*m_ObjectToWorld)(Point3f::zero);
        tkFloat sqrRadius = m_Radius * m_Radius;
        tkFloat sqrDist = squaredDistance(ref.p, center);
        if (sqrDist <= sqrRadius)
            return Shape::getPdf(ref, wi);

        tkFloat cosSqrTheta = 1 - sqrRadius / sqrDist;
        tkFloat cosMaxTheta = std::sqrt(clamp(cosSqrTheta, 0, 1));
        return uniformConePdf(cosMaxTheta);
    }
}  // namespace TK