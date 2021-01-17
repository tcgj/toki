#include "bxdf.hpp"

#include "random.hpp"
#include "spectrum.hpp"
#include "util/scatteringutil.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    bool BxDF::matchesType(BxDFType t) const {
        return (t & m_Type) == m_Type;
    }

    BSDFSample BxDF::sample(const Vec3f& wo, const Vec2f& u, BxDFType type) const {
        if (!(m_Type & type))
            return {};
        Vec3f wi = cosineHemisphereSample(u[0], u[1]);
        if (wo.z < 0)
            wi.z *= -1;
        tkFloat pdf = getPdf(wo, wi, type);
        return BSDFSample(evaluate(wo, wi), wi, pdf);
    }

    tkFloat BxDF::getPdf(const Vec3f& wo, const Vec3f& wi, BxDFType type) const {
        if (!(m_Type & type) || !isSameHemisphere(wo, wi))
            return 0;
        return absCosTheta(wi) * TK_INVPI;
    }
}  // namespace TK
