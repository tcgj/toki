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
        Mesh(const Transform &objectToWorld, tkUInt numTri,
                   const tkUInt *I, tkUInt numVert, const tkPoint3f *V,
                   const tkVec3f *N, const tkVec3f *T);

        bool getTriVertices(tkUInt index, Vertex *v0, Vertex *v1, Vertex *v2) const;

        tkUInt numTri, numVert;
        std::vector<tkUInt> indexBuffer; // std::vector to allow for easy triangle inversion
        std::unique_ptr<tkPoint3f[]> vertexBuffer;
        std::unique_ptr<tkVec3f[]> normalBuffer;
        std::unique_ptr<tkVec3f[]> tangentBuffer;
    };
} // namespace TK