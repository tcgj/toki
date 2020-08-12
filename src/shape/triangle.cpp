#include "triangle.hpp"

#include "mesh.hpp"
#include "core/aabb.hpp"
#include "core/ray.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    inline tkAABBf Triangle::objectBoundingBox() const {
        Vertex v0, v1, v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        return bbUnion(tkAABBf(objectToWorld->applyInverse(v0.pos),
                               objectToWorld->applyInverse(v1.pos)),
                       objectToWorld->applyInverse(v2.pos));
    }

    tkAABBf Triangle::worldBoundingBox() const {
        Vertex v0, v1, v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);
        return bbUnion(tkAABBf(v0.pos, v1.pos), v2.pos);
    }

    tkFloat Triangle::surfaceArea() const {
        Vertex v0, v1, v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        tkVec3f v01 = v1.pos - v0.pos;
        tkVec3f v02 = v2.pos - v0.pos;

        return cross(v01, v02).magnitude() * 0.5;
    }

    // Implementation of Möller–Trumbore Ray-Triangle algorithm from
    // https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    bool Triangle::intersect(const Ray &r, tkFloat *tHit,
                             SurfaceInteraction *interaction) const {
        Vertex v0, v1, v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        tkVec3f e1 = v1.pos - v0.pos;
        tkVec3f e2 = v2.pos - v0.pos;

        tkVec3f p = cross(r.d, e2);
        tkFloat det = dot(e1, p);
        if (std::abs(det) < TK_EPSILON)
            return false;
        tkFloat invDet = 1.0 / det;

        tkVec3f t = r.o - v0.pos;

        // Calculate barycentric coords (u,v)
        tkFloat u = dot(t, p) * invDet;
        if (u < 0.0 || u > 1.0)
            return false;

        tkVec3f q = cross(t, e1);

        tkFloat v = dot(r.d, q) * invDet;
        if (v < 0.0 || u + v > 1.0)
            return false;

        tkFloat tempT = dot(e2, q) * invDet;
        if (tempT < TK_EPSILON || tempT > r.tMax)
            return false;

        tkVec3f normal = cross(e1, e2).normalized();

        *tHit = tempT;
        interaction->p = r(*tHit);
        interaction->n = invertNormals ? -normal : normal;
        interaction->wo = -r.d;
        interaction->shape = this;
        return true;
    }

    bool Triangle::hasIntersect(const Ray &r) const {
        Vertex v0, v1, v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        tkVec3f e1 = v1.pos - v0.pos;
        tkVec3f e2 = v2.pos - v0.pos;

        tkVec3f p = cross(r.d, e2);
        tkFloat det = dot(e1, p);
        if (std::abs(det) < TK_EPSILON)
            return false;
        tkFloat invDet = 1.0 / det;

        tkVec3f t = r.o - v0.pos;

        // Calculate barycentric coords (u,v)
        tkFloat u = dot(t, p) * invDet;
        if (u < 0.0 || u > 1.0)
            return false;

        tkVec3f q = cross(t, e1);

        tkFloat v = dot(r.d, q) * invDet;
        if (v < 0.0 || u + v > 1.0)
            return false;

        tkFloat tempT = dot(e2, q) * invDet;
        if (tempT < TK_EPSILON || tempT > r.tMax)
            return false;

        return true;
    }

    SurfaceInteraction Triangle::sample(const Interaction &ref,
                                        const tkVec2f &samp,
                                        tkFloat *pdf) const {
        SurfaceInteraction ret;

        Vertex a, b, c;
        mesh->getTriVertices(triIndex, &a, &b, &c);
        tkVec2f bCoord = uniformTriangleSample(samp.x, samp.y);
        tkFloat bCoordZ = (1 - bCoord.x - bCoord.y);

        ret.p = bCoord.x * a.pos + bCoord.y * b.pos + bCoordZ * c.pos;
        if (mesh->normalBuffer != nullptr)
            ret.n = bCoord.x * a.normal + bCoord.y * b.normal + bCoordZ * c.normal;
        else
            ret.n = normalize(cross(b.pos - a.pos, c.pos - a.pos));
        if (invertNormals)
            ret.n = -ret.n;
        ret.wo = normalize(ref.p - ret.p);
        *pdf = getPdf(ref, ret);
        return ret;
    }
}  // namespace TK