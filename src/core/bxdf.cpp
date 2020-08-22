#include "bxdf.hpp"

#include "random.hpp"
#include "spectrum.hpp"
#include "util/scatteringutil.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    tkSpectrum BxDF::sample(const tkVec3f &wo, tkVec3f *wi, const tkVec2f &samp,
                            tkFloat *pdf) const {
        *wi = cosineHemisphereSample(samp[0], samp[1]);
        if (wo.z < 0)
            wi->z = -(wi->z);
        *pdf = getPdf(wo, *wi);
        return this->evaluate(wo, *wi);
    }

    bool BxDF::matchesType(BxDFType t) const {
        return (t & type) == type;
    }

    tkFloat BxDF::getPdf(const tkVec3f &wo, const tkVec3f &wi) const {
        return isSameHemisphere(wo, wi) ? absCosTheta(wi) * TK_INVPI : 0;
    }
} // namespace TK
