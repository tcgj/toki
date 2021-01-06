#include "mesh.hpp"

#include "core/transform.hpp"

namespace TK {
    // mesh is initialized in world space to eliminate need for transformation
    // during ray intersection check
    Mesh::Mesh(const Transform& objectToWorld, int64_t numVert, int64_t numTri, const Point3f* V,
               const int64_t* I, const Vec3f* N, const Vec3f* UV)
        : m_NumTri(numTri), m_NumVert(numVert), m_IndexBuffer(I, I + 3 * numTri) {
        // clear ptr and set ownership
        m_VertexBuffer = std::make_unique<Point3f[]>(numVert);
        for (int64_t i = 0; i < numVert; ++i)
            m_VertexBuffer[i] = objectToWorld(V[i]);

        if (N != nullptr) {
            m_NormalBuffer = std::make_unique<Vec3f[]>(numVert);
            for (int64_t i = 0; i < numVert; ++i)
                m_NormalBuffer[i] = objectToWorld(N[i]);
        }
        if (UV != nullptr) {
            m_UvBuffer = std::make_unique<Point2f[]>(numVert);
            memcpy(m_UvBuffer.get(), UV, numVert * sizeof(Point2f));
        }
    }
}  // namespace TK