#pragma once

#include "core/region.hpp"
#include "core/aabb.hpp"
#include "region/primitive.hpp"

namespace TK {
    class Iterator : public Region {
    public:
        Iterator(std::vector<std::shared_ptr<Primitive>>& primitives) : primitives(primitives) {
            for (const auto& p : primitives) {
                worldBB = bbUnion(worldBB, p->worldBoundingBox());
            }
        }

        tkAABBf worldBoundingBox() const override {
            return worldBB;
        }
        bool hasIntersect(const Ray& r) const override {
            for (const auto& prim : primitives) {
                if (prim->hasIntersect(r)) {
                    return true;
                }
            }
            return false;
        }
        bool intersect(const Ray& r, SurfaceInteraction* interaction) const override {
            bool hasIntersected = false;
            for (const auto& prim : primitives) {
                if (prim->intersect(r, interaction)) {
                    hasIntersected = true;
                }
            }
            return hasIntersected;
        }

        std::vector<std::shared_ptr<Primitive>> primitives;
        tkAABBf worldBB;
    };
}  // namespace TK
