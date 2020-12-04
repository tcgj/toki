#include "interaction.hpp"

#include "ray.hpp"
#include "spectrum.hpp"
#include "light.hpp"
#include "scattering.hpp"
#include "region/primitive.hpp"

namespace TK {
    Ray Interaction::spawnRayTo(const Vec3f& d) const {
        Point3f o = p + d * TK_EPSILON;
        return Ray(o, d);
    }

    Ray Interaction::spawnRayTo(const Interaction& ref) const {
        Vec3f d = normalize(ref.p - p);
        Point3f o = p + d * TK_EPSILON;
        Point3f end = ref.p - d * TK_EPSILON;
        return Ray(o, end - o, 1.0f - TK_EPSILON);
    }

    void SurfaceInteraction::computeScattering(Scattering* s) {
        s->initialize(*this);
        primitive->computeScattering(s);
        scattering = s;
    }

    tkSpectrum SurfaceInteraction::Le() const {
        std::shared_ptr<Light> light = primitive->getLight();
        if (light == nullptr)
            return 0;

        return light->Le(*this, wo);
    }
}  // namespace TK
