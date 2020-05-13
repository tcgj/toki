#include "mesh.hpp"

namespace TK {
    // mesh is initialized in world space to eliminate need for transformation
    // during ray intersection check
    Mesh::Mesh(const Transform &worldTransform, tkInt numTri, const tkInt *I,
               tkInt numVert, const tkPoint3f *V, const tkVec3f *N,
               const tkVec3f *T)
        : numTri(numTri), numVert(numVert), indexBuffer(I, I + 3 * numTri) {
        // clear ptr and set ownership
        vertexBuffer.reset(new tkPoint3f[numVert]);
        for (tkInt i = 0; i < numVert; ++i)
            vertexBuffer[i] = worldTransform(V[i]);

        if (N != nullptr) {
            normalBuffer.reset(new tkVec3f[numVert]);
            for (tkInt i = 0; i < numVert; ++i)
                normalBuffer[i] = worldTransform(N[i]);
        }
        if (T != nullptr) {
            tangentBuffer.reset(new tkVec3f[numVert]);
            for (tkInt i = 0; i < numVert; ++i)
                tangentBuffer[i] = worldTransform(T[i]);
        }
    }

    bool Mesh::getTriVertices(tkInt triIndex, VertexAttr *v1, VertexAttr *v2,
                        VertexAttr *v3) const {
        if (triIndex >= numTri)
            return false;

        tkInt i1 = indexBuffer[3 * triIndex];
        tkInt i2 = indexBuffer[3 * triIndex + 1];
        tkInt i3 = indexBuffer[3 * triIndex + 2];

        v1->pos = vertexBuffer[i1];
        v2->pos = vertexBuffer[i2];
        v3->pos = vertexBuffer[i3];
        if (normalBuffer) {
            v1->normal = normalBuffer[i1];
            v2->normal = normalBuffer[i2];
            v3->normal = normalBuffer[i3];
        }
        if (tangentBuffer) {
            v1->tangent = tangentBuffer[i1];
            v2->tangent = tangentBuffer[i2];
            v3->tangent = tangentBuffer[i3];
        }
        return true;
    }
}  // namespace TK