#include "point.hpp"

#include "core/interaction.hpp"

namespace TK {
    bool PointLight::isDelta() const {
        return true;
    }

    tkSpectrum PointLight::power() const {
        return 4 * TK_PI * intensity;
    }

    tkSpectrum PointLight::sample(const Interaction &interaction,
                                    tkVec3f *wi, tkFloat *pdf) const {
        // Manually calculate normalized direction wi to reuse invSqrLen
        tkVec3f dir = pos - interaction.p;
        tkFloat invSqrLen = 1 / dir.squaredMagnitude();
        *wi = dir * std::sqrt(invSqrLen);
        *pdf = 1;

        return intensity * invSqrLen;
    }
} // namespace TK
