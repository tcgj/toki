#pragma once

#include "system/toki.hpp"

namespace TK {
    struct Vertex {
        Point3f p;
        Vec3f n;
        Point2f uv;
    };

    struct Mesh {
        Mesh(const Transform& objectToWorld, int64_t numVert, int64_t numTri, const Point3f* V, const int64_t* I,
             const Vec3f* N, const Vec3f* UV);

        bool getTriVertices(int64_t index, Vertex* v0, Vertex* v1, Vertex* v2) const;

        int64_t numTri;
        int64_t numVert;
        std::vector<int64_t> indexBuffer;  // std::vector to allow for easy triangle inversion
        std::unique_ptr<Point3f[]> vertexBuffer;
        std::unique_ptr<Vec3f[]> normalBuffer;
        std::unique_ptr<Point2f[]> uvBuffer;
    };
}  // namespace TK