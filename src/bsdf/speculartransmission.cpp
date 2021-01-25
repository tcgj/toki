#include "speculartransmission.hpp"

#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum SpecularTransmission::evaluate(const Vec3f& wo, const Vec3f& wi) const {
        return 0;
    }

    BSDFSample SpecularTransmission::sample(const Vec3f& wo, const Vec2f& u, BxDFType type) const {
        if (!(m_Type & type))
            return {};
        bool exiting = cosTheta(wo) > 0;
        tkFloat etaI = exiting ? m_EtaB : m_EtaA;
        tkFloat etaT = exiting ? m_EtaA : m_EtaB;
        Vec3f wi = refract(wo, Vec3f::forward, etaT, etaI);
        if (wi == Vec3f::zero)
            return {};

        tkSpectrum f = m_Dht * (tkSpectrum(1.0f) - m_Fresnel.evaluate(cosTheta(wi))) / absCosTheta(wi);
        return BSDFSample(f, wi, 1);
    }

    tkFloat SpecularTransmission::getPdf(const Vec3f& wo, const Vec3f& wi, BxDFType type) const {
        return 0;
    }
}  // namespace TK
