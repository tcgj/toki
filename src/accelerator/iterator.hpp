#pragma once

#include "core/region.hpp"
#include "core/aabb.hpp"
#include "region/primitive.hpp"

namespace TK {
    class Iterator : public Region {
    public:
        Iterator(std::vector<std::shared_ptr<Primitive>> &primitives,
                 const tkAABBf &worldBB)
            : primitives(primitives), worldBB(worldBB) {}

        tkAABBf worldBoundingBox() const override {
            return worldBB;
        }
        bool hasIntersect(const Ray &r) const override {
            bool hasIntersected = false;
            for (const auto &prim : primitives) {
                if (prim->hasIntersect(r)) {
                    hasIntersected = true;
                    break;
                }
            }
            return hasIntersected;
        }
        bool intersect(const Ray &r,
                       SurfaceInteraction *interaction) const override {
            bool hasIntersected = false;
            for (const auto &prim : primitives) {
                if (prim->intersect(r, interaction)) {
                    hasIntersected = true;
                }
            }
            return hasIntersected;
        }

        std::vector<std::shared_ptr<Primitive>> primitives;
        tkAABBf worldBB;
    };
} // namespace TK
