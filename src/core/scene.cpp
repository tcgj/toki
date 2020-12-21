#include "scene.hpp"

#include "region.hpp"
#include "light.hpp"

namespace TK {
    Scene::Scene(std::shared_ptr<Region> region, std::vector<std::shared_ptr<Light>> lights,
                 std::shared_ptr<Camera> camera, std::shared_ptr<Integrator> integrator)
        : m_Region(std::move(region)),
          m_Lights(std::move(lights)),
          m_Camera(std::move(camera)),
          m_Integrator(std::move(integrator)) {
        m_WorldBB = m_Region->worldBoundingBox();
        // TODO: handle light preprocessing outside constructor
        for (const auto& l : m_Lights) {
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

    Integrator* Scene::getIntegrator() const {
        return m_Integrator.get();
    }
}  // namespace TK
