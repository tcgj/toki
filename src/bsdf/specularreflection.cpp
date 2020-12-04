#include "specularreflection.hpp"

#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum SpecularReflection::evaluate(const Vec3f& wo, const Vec3f& wi) const {
        return 0;
    }

    tkSpectrum SpecularReflection::sample(const Vec3f& wo, Vec3f* wi, const Vec2f& samp,
                                          tkFloat* pdf) const {
        *wi = Vec3f(-wo.x, -wo.y, wo.z);
        *pdf = 1;
        return dhr * fresnel->evaluate(cosTheta(*wi)) / absCosTheta(*wi);
    }

    tkFloat SpecularReflection::getPdf(const Vec3f& wo, const Vec3f& wi) const {
        return 0;
    }
}  // namespace TK
