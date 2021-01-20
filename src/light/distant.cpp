#include "distant.hpp"

#include "core/scene.hpp"

namespace TK {
    void DistantLight::preprocess(const Scene& scene) {
        // Obtain scene radius and scene center from scene data
        scene.worldBoundingBox().boundingSphere(&m_SceneCenter, &m_SceneRadius);
    }

    tkSpectrum DistantLight::power() const {
        // Overestimate of the power received by the scene
        // using a projected disk perpendicular to light direction
        return m_Radiance * TK_PI * m_SceneRadius * m_SceneRadius;
    }

    LightSample DistantLight::sample(const Interaction& ref, const Vec2f& u) const {
        Point3f lightPos = ref.p - 2 * m_SceneRadius * m_Dir;
        return LightSample(m_Radiance, -m_Dir, lightPos, 1);
    }
}  // namespace TK
