#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"

namespace TK {
    struct Vertex {
        tkPoint3f pos;
        tkVec3f normal;
        tkVec3f tangent;
    };

    struct Mesh {
        Mesh(const Transform &objectToWorld, tkI64 numTri,
                   const tkI64 *I, tkI64 numVert, const tkPoint3f *V,
                   const tkVec3f *N, const tkVec3f *T);

        bool getTriVertices(tkI64 index, Vertex *v0, Vertex *v1, Vertex *v2) const;

        tkI64 numTri, numVert;
        std::vector<tkI64> indexBuffer; // std::vector to allow for easy triangle inversion
        std::unique_ptr<tkPoint3f[]> vertexBuffer;
        std::unique_ptr<tkVec3f[]> normalBuffer;
        std::unique_ptr<tkVec3f[]> tangentBuffer;
    };
} // namespace TK