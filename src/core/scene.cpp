#include "scene.hpp"

#include "region.hpp"
#include "light.hpp"

namespace TK {
    Scene::Scene(std::shared_ptr<Region> region, const std::vector<std::shared_ptr<Light>>& lights)
        : region(region), lights(lights) {
        worldBB = region->worldBoundingBox();
        // TODO: handle light preprocessing outside constructor
        for (const auto& l : lights) {
            l->preprocess(*this);
        }
    }

    const tkAABBf& Scene::worldBoundingBox() const {
        return worldBB;
    }

    bool Scene::intersect(const Ray& r, SurfaceInteraction* interaction) const {
        return region->intersect(r, interaction);
    }

    bool Scene::hasIntersect(const Ray& r) const {
        return region->hasIntersect(r);
    }
}  // namespace TK
