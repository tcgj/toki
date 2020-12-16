#include "scene.hpp"

#include "region.hpp"
#include "light.hpp"

namespace TK {
    Scene::Scene(std::shared_ptr<Region> region, const std::vector<std::shared_ptr<Light>>& lights)
        : m_Region(region), m_Lights(lights) {
        m_WorldBB = region->worldBoundingBox();
        // TODO: handle light preprocessing outside constructor
        for (const auto& l : lights) {
            l->preprocess(*this);
        }
    }

    const AABBf& Scene::worldBoundingBox() const {
        return m_WorldBB;
    }

    bool Scene::intersect(const Ray& r, SurfaceInteraction* interaction) const {
        return m_Region->intersect(r, interaction);
    }

    bool Scene::hasIntersect(const Ray& r) const {
        return m_Region->hasIntersect(r);
    }

    Camera* Scene::getCamera() const {
        return m_Camera.get();
    }
}  // namespace TK
