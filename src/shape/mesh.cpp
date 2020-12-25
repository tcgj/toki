#include "mesh.hpp"

#include "core/transform.hpp"

namespace TK {
    // mesh is initialized in world space to eliminate need for transformation
    // during ray intersection check
    Mesh::Mesh(const Transform& objectToWorld, int64_t numVert, int64_t numTri, const Point3f* V,
               const int64_t* I, const Vec3f* N, const Vec3f* UV)
        : m_NumTri(numTri), m_NumVert(numVert), m_IndexBuffer(I, I + 3 * numTri) {
        // clear ptr and set ownership
        m_VertexBuffer.reset(new Point3f[numVert]);
        for (int64_t i = 0; i < numVert; ++i)
            m_VertexBuffer[i] = objectToWorld(V[i]);

        if (N != nullptr) {
            m_NormalBuffer.reset(new Vec3f[numVert]);
            for (int64_t i = 0; i < numVert; ++i)
                m_NormalBuffer[i] = objectToWorld(N[i]);
        }
        if (UV != nullptr) {
            m_UvBuffer.reset(new Point2f[numVert]);
            memcpy(m_UvBuffer.get(), UV, numVert * sizeof(Point2f));
        }
    }

    bool Mesh::getTriVertices(int64_t triIndex, Vertex* v0, Vertex* v1, Vertex* v2) const {
        if (triIndex >= m_NumTri)
            return false;

        int64_t i0 = m_IndexBuffer[3 * triIndex];
        int64_t i1 = m_IndexBuffer[3 * triIndex + 1];
        int64_t i2 = m_IndexBuffer[3 * triIndex + 2];

        v0->p = m_VertexBuffer[i0];
        v1->p = m_VertexBuffer[i1];
        v2->p = m_VertexBuffer[i2];
        if (m_NormalBuffer) {
            v0->n = m_NormalBuffer[i0];
            v1->n = m_NormalBuffer[i1];
            v2->n = m_NormalBuffer[i2];
        }
        if (m_UvBuffer) {
            v0->uv = m_UvBuffer[i0];
            v1->uv = m_UvBuffer[i1];
            v2->uv = m_UvBuffer[i2];
        }
        return true;
    }
}  // namespace TK