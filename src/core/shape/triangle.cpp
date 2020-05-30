#include "triangle.hpp"

#include <vector>

#include "geometry/aabb.hpp"
#include "geometry/mesh.hpp"
#include "geometry/ray.hpp"

namespace TK {
    inline tkAABBf Triangle::objectBoundingBox() const {
        VertexAttr v0, v1, v2;
        Transform worldToObject = inverse(*objectToWorld);
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        return bbUnion(tkAABBf(worldToObject(v0.pos), worldToObject(v1.pos)),
                       worldToObject(v2.pos));
    }

    tkAABBf Triangle::worldBoundingBox() const {
        VertexAttr v0, v1, v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);
        return bbUnion(tkAABBf(v0.pos, v1.pos), v2.pos);
    }

    // Implementation of Möller–Trumbore Ray-Triangle algorithm from
    // https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    bool Triangle::intersect(const Ray &r, tkFloat *tHit,
                             SurfaceInteraction *interaction) const {
        VertexAttr v0, v1, v2;
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
        interaction->wout = -r.d;
        interaction->shape = this;
        return true;
    }
    bool Triangle::hasIntersect(const Ray &r) const {
        VertexAttr v0, v1, v2;
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
        if (v < 0.0 || u + v > 1.0) return false;

        tkFloat tempT = dot(e2, q) * invDet;
        if (tempT < TK_EPSILON || tempT > r.tMax) return false;

        return true;
    }

    std::vector<std::shared_ptr<Shape>> GenerateMesh(
        tkUInt numTri, const tkUInt *I, tkUInt numVert, const tkPoint3f *V,
        const tkVec3f *N, const tkVec3f *T, const Transform *objectToWorld,
        bool invertNormals = false) {
        std::shared_ptr<Mesh> mesh =
            std::make_shared<Mesh>(*objectToWorld, numTri, I, numVert, V, N, T);
        std::vector<std::shared_ptr<Shape>> tris;

        for (tkUInt i = 0; i < numTri; ++i)
            tris.push_back(std::make_shared<Triangle>(mesh, i, objectToWorld,
                                                      invertNormals));

        return tris;
    }
}  // namespace TK