#pragma once

#include "math/math.hpp"
#include "transform.hpp"

namespace TK {
    struct VertexAttr {
        tkPoint3f pos;
        tkVec3f normal;
        tkVec3f tangent;
    };

    struct Mesh {
        Mesh(const Transform &worldTransform, tkUInt numTri,
                   const tkUInt *I, tkUInt numVert, const tkPoint3f *V,
                   const tkVec3f *N, const tkVec3f *T);

        bool getTriVertices(tkUInt index, VertexAttr *v1, VertexAttr *v2, VertexAttr *v3) const;

        tkUInt numTri, numVert;
        std::vector<tkUInt> indexBuffer; // std::vector to allow for easy triangle inversion
        std::unique_ptr<tkPoint3f[]> vertexBuffer;
        std::unique_ptr<tkVec3f[]> normalBuffer;
        std::unique_ptr<tkVec3f[]> tangentBuffer;
    };
} // namespace TK