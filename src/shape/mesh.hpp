#pragma once

#include "system/toki.hpp"

namespace TK {
    struct Vertex {
        Point3f p;
        Vec3f n;
        Point2f uv;
    };

    struct Mesh {
        Mesh(const Transform& objectToWorld, std::vector<int64_t> I, const std::vector<Point3f>& V,
             const std::vector<Vec3f>& N, const std::vector<Point2f>& UV);

        int64_t m_NumTri;
        int64_t m_NumVert;
        std::vector<int64_t> m_IBuf;
        std::unique_ptr<Point3f[]> m_VBuf;
        std::unique_ptr<Vec3f[]> m_NBuf;
        std::unique_ptr<Point2f[]> m_UvBuf;
    };
}  // namespace TK