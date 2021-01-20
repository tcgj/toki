#include "point.hpp"

#include "core/interaction.hpp"

namespace TK {
    tkSpectrum PointLight::power() const {
        return 4 * TK_PI * m_Intensity;
    }

    LightSample PointLight::sample(const Interaction& ref, const Vec2f& u) const {
        // Manually calculate normalized direction wi to reuse invSqrLen
        Vec3f dir = m_Pos - ref.p;
        tkFloat invSqrLen = 1 / dir.squaredMagnitude();
        Vec3f wi = dir * std::sqrt(invSqrLen);

        return LightSample(m_Intensity * invSqrLen, wi, m_Pos, 1);
    }
}  // namespace TK
