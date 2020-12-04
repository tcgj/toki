#include "mesh.hpp"

#include "core/transform.hpp"

namespace TK {
    // mesh is initialized in world space to eliminate need for transformation
    // during ray intersection check
    Mesh::Mesh(const Transform& objectToWorld, int64_t numVert, int64_t numTri, const Point3f* V,
               const int64_t* I, const Vec3f* N, const Vec3f* UV)
        : numTri(numTri), numVert(numVert), indexBuffer(I, I + 3 * numTri) {
        // clear ptr and set ownership
        vertexBuffer.reset(new Point3f[numVert]);
        for (int64_t i = 0; i < numVert; ++i)
            vertexBuffer[i] = objectToWorld(V[i]);

        if (N != nullptr) {
            normalBuffer.reset(new Vec3f[numVert]);
            for (int64_t i = 0; i < numVert; ++i)
                normalBuffer[i] = objectToWorld(N[i]);
        }
        if (UV != nullptr) {
            uvBuffer.reset(new Point2f[numVert]);
            memcpy(uvBuffer.get(), UV, numVert * sizeof(Point2f));
        }
    }

    bool Mesh::getTriVertices(int64_t triIndex, Vertex* v0, Vertex* v1, Vertex* v2) const {
        if (triIndex >= numTri)
            return false;

        int64_t i0 = indexBuffer[3 * triIndex];
        int64_t i1 = indexBuffer[3 * triIndex + 1];
        int64_t i2 = indexBuffer[3 * triIndex + 2];

        v0->p = vertexBuffer[i0];
        v1->p = vertexBuffer[i1];
        v2->p = vertexBuffer[i2];
        if (normalBuffer) {
            v0->n = normalBuffer[i0];
            v1->n = normalBuffer[i1];
            v2->n = normalBuffer[i2];
        }
        if (uvBuffer) {
            v0->uv = uvBuffer[i0];
            v1->uv = uvBuffer[i1];
            v2->uv = uvBuffer[i2];
        }
        return true;
    }
}  // namespace TK