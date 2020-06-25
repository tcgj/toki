#pragma once

#include <vector>

#include "core/region.hpp"
#include "math/math.hpp"
#include "core/aabb.hpp"

namespace TK {
    class BVH : public Region {
    public:
        enum class ConstructionType { SAH, HLBVH, MIDDLE, EQUALCOUNTS };

        struct PrimitiveData {
            PrimitiveData(tkUInt pIndex, const tkAABBf &bb)
                : pIndex(pIndex), bb(bb), centroid(0.5 * (bb.minPt + bb.maxPt)) {}

            tkUInt pIndex;
            tkAABBf bb;
            tkPoint3f centroid;
        };

        struct Node {
            ~Node() {
                delete left;
                delete right;
            }

            void makeLeaf(tkUInt count, tkUInt offset, const tkAABBf &bb);
            void makeInterior(tkUInt axis, Node *node0, Node *node1);

            tkUInt numPrim;
            tkAABBf boundingBox;
            // Used by leaf nodes only
            tkUInt primOffset;
            // Used by interior nodes only
            tkUInt splitAxis;
            Node *left = nullptr;
            Node *right = nullptr;
        };

        BVH(std::vector<std::shared_ptr<Primitive>> &primitives,
            ConstructionType type = ConstructionType::SAH);

        tkAABBf worldBoundingBox() const override;
        bool hasIntersect(const Ray &r) const override;
        bool intersect(const Ray &r, SurfaceInteraction *interaction) const override;

        Node *constructBySAH(
            std::vector<PrimitiveData> &primData, tkUInt start, tkUInt end,
            std::vector<std::shared_ptr<Primitive>> &primOrdered);

    private:
        bool intersectNode(const Ray &r, Node *node, SurfaceInteraction *interaction = nullptr) const;

        Node *root = nullptr;
        std::vector<std::shared_ptr<Primitive>> primitives;
    };

    inline void BVH::Node::makeLeaf(tkUInt count, tkUInt offset, const tkAABBf &bb) {
        numPrim = count;
        primOffset = offset;
        boundingBox = bb;
        left = nullptr;
        right = nullptr;
    }

    inline void BVH::Node::makeInterior(tkUInt axis, Node *node0, Node *node1) {
        numPrim = 0;
        boundingBox = bbUnion(node0->boundingBox, node1->boundingBox);
        splitAxis = axis;
        left = node0;
        right = node1;
    }
} // namespace TK