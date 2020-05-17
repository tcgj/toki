#pragma once

#include "system/system.hpp"

namespace TK {
    class Region {
    public:
        virtual tkAABBf worldBoundingBox() const = 0;
        virtual bool intersect(const Ray &r, SurfaceInteraction *interaction) const = 0;
        virtual bool hasIntersect(const Ray &r) const = 0;

    };
}  // namespace TK