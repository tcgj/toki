#pragma once

#include "core/region.hpp"
#include "core/aabb.hpp"

namespace TK {
    class BVH : public Region {
    public:
        enum Strategy { BINNED_SAH };

        BVH(const std::vector<std::shared_ptr<Primitive>> &primitives,
            Strategy type = BINNED_SAH);

        tkAABBf worldBoundingBox() const override;
        bool hasIntersect(const Ray &r) const override;
        bool intersect(const Ray &r, SurfaceInteraction *interaction) const override;

    private:
        struct PrimitiveUnit {
            PrimitiveUnit(tkI64 index, const tkAABBf &bb)
                : index(index), bb(bb), centroid(bb.center()) {}

            tkI64 index;
            tkAABBf bb;
            tkPoint3f centroid;
        };

        struct Node {
            void makeInner(const tkAABBf &nodeBB, tkI64 nLeft, tkI64 nRight, tkInt splitAxis) {
                bb = nodeBB;
                left = nLeft;
                right = nRight;
                axis = splitAxis;
            }
            void makeLeaf(const tkAABBf &nodeBB, tkI64 pCount, tkI64 pOffset) {
                bb = nodeBB;
                count = pCount;
                offset = pOffset;
            }

            // Common members
            tkAABBf bb;
            // Inner node only
            tkI64 left;
            tkI64 right;
            tkInt axis;
            // Leaf node only
            tkI64 count = 0;
            tkI64 offset;
        };

        // Stack-based BVH traversal
        bool intersectNode(const Ray &r, tkI64 nodeIndex, SurfaceInteraction *interaction = nullptr) const;

        // BVH Construction Functions
        void buildSAH(tkI64 nodeIndex, tkI64 start, tkI64 end,
                      std::vector<PrimitiveUnit> &pSet);

        // BVH arrays
        std::unique_ptr<Node[]> nodes = nullptr;
        std::vector<std::shared_ptr<Primitive>> primitives;
    };
} // namespace TK