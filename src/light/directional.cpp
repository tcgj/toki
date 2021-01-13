#include "directional.hpp"

#include "core/scene.hpp"

namespace TK {
    void DirectionalLight::preprocess(const Scene& scene) {
        // Obtain scene radius and scene center from scene data
        scene.worldBoundingBox().boundingSphere(&m_SceneCenter, &m_SceneRadius);
    }

    tkSpectrum DirectionalLight::power() const {
        // Overestimate of the power received by the scene
        // using a projected disk perpendicular to light direction
        return m_Radiance * TK_PI * m_SceneRadius * m_SceneRadius;
    }

    tkSpectrum DirectionalLight::sample(const Interaction& ref, const Vec2f& u, Vec3f& out_wi,
                                        tkFloat& out_pdf, OcclusionChecker& out_checker) const {
        out_wi = -m_Dir;
        out_pdf = 1;
        Point3f lightRef = ref.p - 2 * m_SceneRadius * m_Dir;
        out_checker = OcclusionChecker(ref, Interaction(lightRef));

        return m_Radiance;
    }
}  // namespace TK
