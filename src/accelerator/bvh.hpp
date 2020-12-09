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
        struct Node;

        // Stack-based BVH traversal
        bool intersectNode(const Ray& r, int64_t nodeIndex, SurfaceInteraction* interaction = nullptr) const;

        // BVH Construction Functions
        void buildSAH(int64_t nodeIndex, int64_t start, int64_t end, std::vector<PrimitiveUnit>& pSet);

        // BVH arrays
        std::unique_ptr<Node[]> nodes = nullptr;
        std::vector<std::shared_ptr<Primitive>> primitives;
    };
}  // namespace TK
