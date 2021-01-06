#include "interaction.hpp"

#include "ray.hpp"
#include "spectrum.hpp"
#include "light.hpp"
#include "bsdf.hpp"
#include "region/primitive.hpp"
#include "util/fputil.hpp"

namespace TK {
    constexpr tkFloat originOffsetLimit = 1.0f / 32.0f;
    constexpr tkFloat intOffsetScale = 256.0f;
    constexpr tkFloat floatOffsetScale = 1.0f / 65536.0f;

    Ray Interaction::spawnRayTo(const Vec3f& d) const {
        return Ray(p, d);
    }

    Ray Interaction::spawnRayTo(const Interaction& ref) const {
        Vec3f d = ref.p - p;
        return Ray(p, d, 1.0f - TK_EPSILON);
    }

    Point3f SurfaceInteraction::offsetRayOrigin(const Point3f& p, const Vec3f& n) const {
        Vec3i offset(intOffsetScale * n.x, intOffsetScale * n.y, intOffsetScale * n.z);

        Point3f pInt(intToFloat(floatToInt(p.x) + (p.x < 0 ? -offset.x : offset.x)),
                   intToFloat(floatToInt(p.y) + (p.y < 0 ? -offset.y : offset.y)),
                   intToFloat(floatToInt(p.z) + (p.z < 0 ? -offset.z : offset.z)));

        return Point3f(std::abs(p.x) < originOffsetLimit ? p.x + floatOffsetScale * n.x : pInt.x,
                     std::abs(p.y) < originOffsetLimit ? p.y + floatOffsetScale * n.y : pInt.y,
                     std::abs(p.z) < originOffsetLimit ? p.z + floatOffsetScale * n.z : pInt.z);
    }

    Ray SurfaceInteraction::spawnRayTo(const Vec3f& d) const {
        Point3f o = offsetRayOrigin(p, n);
        return Ray(o, d);
    }

    Ray SurfaceInteraction::spawnRayTo(const Interaction& ref) const {
        Point3f o = offsetRayOrigin(p, n);
        Vec3f d = ref.p - o;
        return Ray(o, d, 1.0f - TK_EPSILON);
    }

    void SurfaceInteraction::computeScattering(BSDF* s) {
        s->initialize(*this);
        primitive->computeScattering(s);
        bsdf = s;
    }

    tkSpectrum SurfaceInteraction::Le() const {
        std::shared_ptr<Light> light = primitive->getLight();
        if (light == nullptr)
            return 0;

        return light->Le(*this, wo);
    }
}  // namespace TK
