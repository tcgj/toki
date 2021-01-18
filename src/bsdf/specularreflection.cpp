#include "specularreflection.hpp"

#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum SpecularReflection::evaluate(const Vec3f& wo, const Vec3f& wi) const {
        return 0;
    }

    BSDFSample SpecularReflection::sample(const Vec3f& wo, const Vec2f& u, BxDFType type) const {
        if (!(m_Type & type))
            return {};

        Vec3f wi = Vec3f(-wo.x, -wo.y, wo.z);
        tkSpectrum f = m_Dhr * m_Fresnel->evaluate(cosTheta(wi)) / absCosTheta(wi);
        return BSDFSample(f, wi, 1);
    }

    tkFloat SpecularReflection::getPdf(const Vec3f& wo, const Vec3f& wi, BxDFType type) const {
        return 0;
    }
}  // namespace TK
