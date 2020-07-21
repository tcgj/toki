#pragma once

#include "system/toki.hpp"
#include "aabb.hpp"

namespace TK {
    class Scene {
    public:
        Scene(std::shared_ptr<Region> region, const std::vector<std::shared_ptr<Light>> &lights);

        const tkAABBf &worldBoundingBox() const;
        bool intersect(const Ray &r, SurfaceInteraction *interaction) const;
        bool hasIntersect(const Ray &r) const;

        std::vector<std::shared_ptr<Light>> lights;
    private:
        std::shared_ptr<Region> region;
        tkAABBf worldBB;
    };
} // namespace TK
