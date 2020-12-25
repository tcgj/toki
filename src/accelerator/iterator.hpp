#pragma once

#include "core/region.hpp"
#include "core/aabb.hpp"
#include "region/primitive.hpp"

namespace TK {
    class Iterator : public Region {
    public:
        Iterator(std::vector<std::shared_ptr<Primitive>>& primitives) : m_Primitives(primitives) {
            for (const auto& p : primitives) {
                m_WorldBB = bbUnion(m_WorldBB, p->worldBoundingBox());
            }
        }

        AABBf worldBoundingBox() const override {
            return m_WorldBB;
        }

        bool hasIntersect(const Ray& r) const override {
            for (const auto& prim : m_Primitives) {
                if (prim->hasIntersect(r)) {
                    return true;
                }
            }
            return false;
        }

        bool intersect(const Ray& r, SurfaceInteraction* interaction) const override {
            bool hasIntersected = false;
            for (const auto& prim : m_Primitives) {
                if (prim->intersect(r, interaction)) {
                    hasIntersected = true;
                }
            }
            return hasIntersected;
        }

        std::vector<std::shared_ptr<Primitive>> m_Primitives;
        AABBf m_WorldBB;
    };
}  // namespace TK
