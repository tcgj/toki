#include "triangle.hpp"

#include "mesh.hpp"
#include "core/aabb.hpp"
#include "core/ray.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    tkAABBf Triangle::objectBoundingBox() const {
        Vertex v0;
        Vertex v1;
        Vertex v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        return bbUnion(tkAABBf(objectToWorld->applyInverse(v0.p), objectToWorld->applyInverse(v1.p)),
                       objectToWorld->applyInverse(v2.p));
    }

    tkAABBf Triangle::worldBoundingBox() const {
        Vertex v0;
        Vertex v1;
        Vertex v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);
        return bbUnion(tkAABBf(v0.p, v1.p), v2.p);
    }

    tkFloat Triangle::surfaceArea() const {
        Vertex v0;
        Vertex v1;
        Vertex v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        tkVec3f v01 = v1.p - v0.p;
        tkVec3f v02 = v2.p - v0.p;

        return cross(v01, v02).magnitude() * 0.5;
    }

    // Implementation of Möller–Trumbore Ray-Triangle algorithm from
    // https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    bool Triangle::intersect(const Ray& r, tkFloat* tHit, SurfaceInteraction* interaction) const {
        Vertex v0;
        Vertex v1;
        Vertex v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        tkVec3f e1 = v1.p - v0.p;
        tkVec3f e2 = v2.p - v0.p;

        tkVec3f p = cross(r.d, e2);
        tkFloat det = dot(e1, p);
        if (std::abs(det) < TK_EPSILON)
            return false;
        tkFloat invDet = 1.0 / det;

        tkVec3f t = r.o - v0.p;

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

        tkVec2f duv1 = v1.uv - v0.uv;
        tkVec2f duv2 = v2.uv - v0.uv;
        tkFloat uvDet = duv1[0] * duv2[1] - duv1[1] * duv2[0];
        tkVec3f tangent;
        tkVec3f bitangent;
        if (uvDet == 0)
            coordinateSystem(normal, &tangent, &bitangent);
        else {
            tkFloat invDet = 1 / uvDet;
            tangent = normalize((duv2[1] * e1 - duv1[1] * e2) * invDet);
            bitangent = normalize((duv1[0] * e2 - duv2[0] * e1) * invDet);
        }

        *tHit = tempT;
        interaction->p = v0.p * (1 - u - v) + v1.p * u + v2.p * v;
        interaction->n = invertNormals ? -normal : normal;
        interaction->dpdu = tangent;
        interaction->dpdv = bitangent;
        interaction->wo = -r.d;
        interaction->shape = this;
        return true;
    }

    bool Triangle::hasIntersect(const Ray& r) const {
        Vertex v0;
        Vertex v1;
        Vertex v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        tkVec3f e1 = v1.p - v0.p;
        tkVec3f e2 = v2.p - v0.p;

        tkVec3f p = cross(r.d, e2);
        tkFloat det = dot(e1, p);
        if (std::abs(det) < TK_EPSILON)
            return false;
        tkFloat invDet = 1.0 / det;

        tkVec3f t = r.o - v0.p;

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

    SurfaceInteraction Triangle::sample(const Interaction& ref, const tkVec2f& samp, tkFloat* pdf) const {
        SurfaceInteraction ret;

        Vertex v0;
        Vertex v1;
        Vertex v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);
        tkVec2f bCoord = uniformTriangleSample(samp[0], samp[1]);
        tkFloat bCoordZ = (1 - bCoord.x - bCoord.y);

        ret.p = bCoord.x * v0.p + bCoord.y * v1.p + bCoordZ * v2.p;
        if (mesh->normalBuffer != nullptr)
            ret.n = bCoord.x * v0.n + bCoord.y * v1.n + bCoordZ * v2.n;
        else
            ret.n = normalize(cross(v1.p - v0.p, v2.p - v0.p));
        if (invertNormals)
            ret.n = -ret.n;
        ret.wo = normalize(ref.p - ret.p);
        *pdf = getPdf(ref, ret);
        return ret;
    }
}  // namespace TK