#include "mesh.hpp"

#include "core/transform.hpp"

namespace TK {
    Mesh::Mesh(const Transform& objectToWorld, std::vector<int64_t> I, const std::vector<Point3f>& V,
                 const std::vector<Vec3f>& N, const std::vector<Point2f>& UV)
        : m_NumTri(I.size() / 3), m_NumVert(V.size()), m_IBuf(std::move(I)) {
        m_VBuf = std::make_unique<Point3f[]>(m_NumVert);
        for (int64_t i = 0; i < m_NumVert; ++i)
            m_VBuf[i] = objectToWorld(V[i]);

        if (!N.empty()) {
            m_NBuf = std::make_unique<Vec3f[]>(m_NumVert);
            for (int64_t i = 0; i < m_NumVert; ++i)
                m_NBuf[i] = objectToWorld(N[i], true);
        }
        if (!UV.empty()) {
            m_UvBuf = std::make_unique<Point2f[]>(m_NumVert);
            memcpy(m_UvBuf.get(), UV.data(), m_NumVert * sizeof(Point2f));
        }
    }
}  // namespace TK