#include "triangle.hpp"

namespace TK {
    tkAABBf Triangle::objectBoundingBox() const {
        VertexAttr v0, v1, v2;
        Transform worldToObject = inverse(*worldTransform);
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);

        return bbUnion(tkAABBf(worldToObject(v0.pos), worldToObject(v1.pos)), worldToObject(v2.pos));
    }

    tkAABBf Triangle::worldBoundingBox() const {
        VertexAttr v0, v1, v2;
        mesh->getTriVertices(triIndex, &v0, &v1, &v2);
        return bbUnion(tkAABBf(v0.pos, v1.pos), v2.pos);
    }

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

        *tHit = tempT;
        interaction->p = r(*tHit);
        interaction->n = cross(e1, e2).normalized();
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
        if (v < 0.0 || u + v > 1.0)
            return false;

        tkFloat tempT = dot(e2, q) * invDet;
        if (tempT < TK_EPSILON || tempT > r.tMax)
            return false;

        return true;
    }

    std::vector<std::shared_ptr<Shape>> GenerateMesh(
        tkInt numTri, const tkInt *I, tkInt numVert, const tkPoint3f *V,
        const tkVec3f *N, const tkVec3f *T, const Transform *worldTransform,
        bool invertNormals = false) {
        std::shared_ptr<Mesh> mesh =
            std::make_shared<Mesh>(worldTransform, numTri, I, numVert, V, N, T);
        std::vector<std::shared_ptr<Shape>> tris;

        for (tkInt i = 0; i < numTri; ++i)
            tris.push_back(std::make_shared<Triangle>(mesh, i, worldTransform, invertNormals));

        return tris;
    }
} // namespace TK