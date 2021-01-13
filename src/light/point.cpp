#include "point.hpp"

#include "core/interaction.hpp"

namespace TK {
    tkSpectrum PointLight::power() const {
        return 4 * TK_PI * m_Intensity;
    }

    tkSpectrum PointLight::sample(const Interaction& ref, const Vec2f& u, Vec3f& out_wi,
                                        tkFloat& out_pdf, OcclusionChecker& out_checker) const {
        // Manually calculate normalized direction wi to reuse invSqrLen
        Vec3f dir = m_Pos - ref.p;
        tkFloat invSqrLen = 1 / dir.squaredMagnitude();
        out_wi = dir * std::sqrt(invSqrLen);
        out_pdf = 1;
        out_checker = OcclusionChecker(ref, Interaction(m_Pos));

        return m_Intensity * invSqrLen;
    }
}  // namespace TK
