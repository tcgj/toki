#include "bxdf.hpp"

#include "scatteringutil.hpp"
#include "sampling/random.hpp"
#include "sampling/sampling.hpp"
#include "spectrum/rgbspectrum.hpp"

namespace TK {
    bool BxDF::hasType(BxDFType t) const {
        return (t & type) == type;
    }

    tkFloat BxDF::getPdf(const tkVec3f &wo, const tkVec3f &wi) const {
        return isSameHemisphere(wo, wi) ? absCosTheta(wi) * TK_INVPI : 0;
    }

    tkSpectrum BxDF::getSample(const tkVec3f &wo, tkVec3f *wi,
                                 tkFloat *pdf) const {
        *wi = cosHemisphereSample(Random::nextFloat(), Random::nextFloat());
        if (wo.z < 0)
            wi->z = -(wi->z);
        *pdf = getPdf(wo, *wi);
        return (*this)(wo, *wi);
    }
} // namespace TK
