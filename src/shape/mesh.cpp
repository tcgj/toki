#include "mesh.hpp"

#include "core/transform.hpp"

namespace TK {
    // mesh is initialized in world space to eliminate need for transformation
    // during ray intersection check
    Mesh::Mesh(const Transform &objectToWorld, tkUInt numTri, const tkUInt *I,
               tkUInt numVert, const tkPoint3f *V, const tkVec3f *N,
               const tkVec3f *T)
        : numTri(numTri), numVert(numVert), indexBuffer(I, I + 3 * numTri) {
        // clear ptr and set ownership
        vertexBuffer.reset(new tkPoint3f[numVert]);
        for (tkUInt i = 0; i < numVert; ++i)
            vertexBuffer[i] = objectToWorld(V[i]);

        if (N != nullptr) {
            normalBuffer.reset(new tkVec3f[numVert]);
            for (tkUInt i = 0; i < numVert; ++i)
                normalBuffer[i] = objectToWorld(N[i]);
        }
        if (T != nullptr) {
            tangentBuffer.reset(new tkVec3f[numVert]);
            for (tkUInt i = 0; i < numVert; ++i)
                tangentBuffer[i] = objectToWorld(T[i]);
        }
    }

    bool Mesh::getTriVertices(tkUInt triIndex, Vertex *v0, Vertex *v1,
                        Vertex *v2) const {
        if (triIndex >= numTri)
            return false;

        tkUInt i0 = indexBuffer[3 * triIndex];
        tkUInt i1 = indexBuffer[3 * triIndex + 1];
        tkUInt i2 = indexBuffer[3 * triIndex + 2];

        v0->pos = vertexBuffer[i0];
        v1->pos = vertexBuffer[i1];
        v2->pos = vertexBuffer[i2];
        if (normalBuffer) {
            v0->normal = normalBuffer[i0];
            v1->normal = normalBuffer[i1];
            v2->normal = normalBuffer[i2];
        }
        if (tangentBuffer) {
            v0->tangent = tangentBuffer[i0];
            v1->tangent = tangentBuffer[i1];
            v2->tangent = tangentBuffer[i2];
        }
        return true;
    }
}  // namespace TK