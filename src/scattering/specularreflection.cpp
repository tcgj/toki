#include "specularreflection.hpp"

#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum SpecularReflection::evaluate(const tkVec3f& wo, const tkVec3f& wi) const {
        return 0;
    }

    tkSpectrum SpecularReflection::sample(const tkVec3f& wo, tkVec3f* wi, const tkVec2f& samp,
                                          tkFloat* pdf) const {
        *wi = tkVec3f(-wo.x, -wo.y, wo.z);
        *pdf = 1;
        return dhr * fresnel->evaluate(cosTheta(*wi)) / absCosTheta(*wi);
    }

    tkFloat SpecularReflection::getPdf(const tkVec3f& wo, const tkVec3f& wi) const {
        return 0;
    }
}  // namespace TK
