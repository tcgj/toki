#include "point.hpp"

#include "core/interaction.hpp"

namespace TK {
    tkSpectrum PointLight::power() const {
        return 4 * TK_PI * intensity;
    }

    tkSpectrum PointLight::sample(const Interaction& ref, Vec3f* wi, const Vec2f& samp, tkFloat* pdf,
                                  OcclusionChecker* occCheck) const {
        // Manually calculate normalized direction wi to reuse invSqrLen
        Vec3f dir = pos - ref.p;
        tkFloat invSqrLen = 1 / dir.squaredMagnitude();
        *wi = dir * std::sqrt(invSqrLen);
        *pdf = 1;
        *occCheck = OcclusionChecker(ref, Interaction(pos));

        return intensity * invSqrLen;
    }
}  // namespace TK
