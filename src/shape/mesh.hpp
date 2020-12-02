#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"

namespace TK {
    struct Vertex {
        tkPoint3f p;
        tkVec3f n;
        tkPoint2f uv;
    };

    struct Mesh {
        Mesh(const Transform& objectToWorld, tkI64 numVert, tkI64 numTri, const tkPoint3f* V, const tkI64* I,
             const tkVec3f* N, const tkVec3f* UV);

        bool getTriVertices(tkI64 index, Vertex* v0, Vertex* v1, Vertex* v2) const;

        tkI64 numTri;
        tkI64 numVert;
        std::vector<tkI64> indexBuffer;  // std::vector to allow for easy triangle inversion
        std::unique_ptr<tkPoint3f[]> vertexBuffer;
        std::unique_ptr<tkVec3f[]> normalBuffer;
        std::unique_ptr<tkPoint2f[]> uvBuffer;
    };
}  // namespace TK