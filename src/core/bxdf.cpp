#include "bxdf.hpp"

#include "random.hpp"
#include "spectrum.hpp"
#include "util/scatteringutil.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    bool BxDF::matchesType(BxDFType t) const {
        return (t & type) == type;
    }

    tkSpectrum BxDF::sample(const Vec3f& wo, Vec3f* wi, const Vec2f& samp, tkFloat* pdf) const {
        *wi = cosineHemisphereSample(samp[0], samp[1]);
        if (wo.z < 0)
            wi->z = -(wi->z);
        *pdf = getPdf(wo, *wi);
        return this->evaluate(wo, *wi);
    }

    tkFloat BxDF::getPdf(const Vec3f& wo, const Vec3f& wi) const {
        return isSameHemisphere(wo, wi) ? absCosTheta(wi) * TK_INVPI : 0;
    }
}  // namespace TK
