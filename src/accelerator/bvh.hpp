#pragma once

#include "core/region.hpp"
#include "core/aabb.hpp"

namespace TK {
    class BVH : public Region {
    public:
        enum class Strategy { BinnedSAH };

        BVH(const std::vector<std::shared_ptr<Primitive>> &primitives,
            Strategy type = Strategy::BinnedSAH);

        tkAABBf worldBoundingBox() const override;
        bool hasIntersect(const Ray &r) const override;
        bool intersect(const Ray &r, SurfaceInteraction *interaction) const override;

    private:
        struct PrimitiveUnit {
            PrimitiveUnit(tkUInt index, const tkAABBf &bb)
                : index(index), bb(bb), centroid(bb.center()) {}

            tkUInt index;
            tkAABBf bb;
            tkPoint3f centroid;
        };

        struct Node {
            void makeInner(const tkAABBf &nodeBB, tkUInt nLeft, tkUInt nRight, tkInt splitAxis) {
                bb = nodeBB;
                left = nLeft;
                right = nRight;
                axis = splitAxis;
            }
            void makeLeaf(const tkAABBf &nodeBB, tkUInt pCount, tkUInt pOffset) {
                bb = nodeBB;
                count = pCount;
                offset = pOffset;
            }

            // Common members
            tkAABBf bb;
            // Inner node only
            tkUInt left;
            tkUInt right;
            tkInt axis;
            // Leaf node only
            tkUInt count = 0;
            tkUInt offset;
        };

        // Stack-based BVH traversal
        bool intersectNode(const Ray &r, tkUInt nodeIndex, SurfaceInteraction *interaction = nullptr) const;

        // BVH Construction Functions
        void buildSAH(tkUInt nodeIndex, tkUInt start, tkUInt end,
                      std::vector<PrimitiveUnit> &pSet,
                      std::vector<std::shared_ptr<Primitive>> &pSorted);

        // BVH arrays
        std::unique_ptr<Node[]> nodes = nullptr;
        std::vector<std::shared_ptr<Primitive>> primitives;
    };
} // namespace TK