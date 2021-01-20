#pragma once

#include "system/toki.hpp"

namespace TK {
    struct Vertex {
        Point3f p;
        Vec3f n;
        Point2f uv;
    };

    struct Mesh {
        Mesh(const Transform& objectToWorld, int64_t numVert, int64_t numTri, const Point3f* V,
             const int64_t* I, const Vec3f* N, const Vec3f* UV);

        int64_t m_NumTri;
        int64_t m_NumVert;
        std::vector<int64_t> m_IndexBuffer;  // std::vector to allow for easy triangle inversion
        std::unique_ptr<Point3f[]> m_VertexBuffer;
        std::unique_ptr<Vec3f[]> m_NormalBuffer;
        std::unique_ptr<Point2f[]> m_UvBuffer;
    };
}  // namespace TK