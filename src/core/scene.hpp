#pragma once

#include "system/toki.hpp"
#include "aabb.hpp"

namespace TK {
    class Scene {
    public:
        Scene(std::shared_ptr<Region> region, std::vector<std::shared_ptr<Light>> lights,
              std::shared_ptr<Camera> camera, std::shared_ptr<Integrator> integrator);

        const AABBf& worldBoundingBox() const;

        bool intersect(const Ray& r, SurfaceInteraction& out_its) const;

        bool hasIntersect(const Ray& r) const;

        Camera* getCamera() const;

        Integrator* getIntegrator() const;

        std::vector<std::shared_ptr<Light>> m_Lights;

    private:
        std::shared_ptr<Region> m_Region;
        std::shared_ptr<Camera> m_Camera;
        std::shared_ptr<Integrator> m_Integrator;
        AABBf m_WorldBB;
    };
}  // namespace TK
