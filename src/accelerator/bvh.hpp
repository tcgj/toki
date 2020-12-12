#pragma once

#include "core/region.hpp"
#include "core/aabb.hpp"

namespace TK {
    class BVH : public Region {
    public:
        enum Strategy { BINNED_SAH };

        BVH(const std::vector<std::shared_ptr<Primitive>>& primitives, Strategy type = BINNED_SAH);

        AABBf worldBoundingBox() const override;

        bool hasIntersect(const Ray& r) const override;

        bool intersect(const Ray& r, SurfaceInteraction* interaction) const override;

    private:
        struct PrimitiveUnit;
        struct Node {
            void makeInner(const AABBf& nodeBB, int64_t nLeft, int64_t nRight, int splitAxis) {
                bb = nodeBB;
                left = nLeft;
                right = nRight;
                axis = splitAxis;
            }

            void makeLeaf(const AABBf& nodeBB, int64_t pCount, int64_t pOffset) {
                bb = nodeBB;
                count = pCount;
                offset = pOffset;
            }

            // Common members
            AABBf bb;
            // Inner node only
            int64_t left;
            int64_t right;
            int axis;
            // Leaf node only
            int64_t count = 0;
            int64_t offset;
        };

        // Stack-based BVH traversal
        bool intersectNode(const Ray& r, int64_t nodeIndex, SurfaceInteraction* interaction = nullptr) const;

        // BVH Construction Functions
        void buildSAH(int64_t nodeIndex, int64_t start, int64_t end, std::vector<PrimitiveUnit>& pSet);

        // BVH arrays
        std::unique_ptr<Node[]> nodes = nullptr;
        std::vector<std::shared_ptr<Primitive>> primitives;
    };
}  // namespace TK
