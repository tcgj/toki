#pragma once

#include "system/toki.hpp"

namespace TK {
    class Region {
    public:
        virtual ~Region() = default;

        virtual tkAABBf worldBoundingBox() const = 0;
        virtual bool intersect(const Ray& r, SurfaceInteraction* interaction) const = 0;
        virtual bool hasIntersect(const Ray& r) const = 0;
    };
}  // namespace TK