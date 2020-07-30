#include "interaction.hpp"

#include "ray.hpp"
#include "spectrum.hpp"
#include "light.hpp"
#include "region/primitive.hpp"

namespace TK {
    Ray Interaction::spawnRayTo(const tkVec3f &d) const {
        tkPoint3f o = p + d * TK_EPSILON;
        return Ray(o, d);
    }

    Ray Interaction::spawnRayTo(const Interaction &ref) const {
        tkVec3f d = normalize(ref.p - p);
        tkPoint3f o = p + d * TK_EPSILON;
        tkPoint3f end = ref.p - d * TK_EPSILON;
        return Ray(o, end - o, 1.0f - TK_EPSILON);
    }

    void SurfaceInteraction::computeScattering(Scattering *s) {
        primitive->computeScattering(s);
        scattering = s;
    }

    tkSpectrum SurfaceInteraction::computeLe() const {
        std::shared_ptr<Light> light = primitive->getLight();
        if (light == nullptr)
            return 0;

        return light->computeLe(*this, wo);
    }
} // namespace TK
