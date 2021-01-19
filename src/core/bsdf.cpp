#include "bsdf.hpp"

#include "interaction.hpp"
#include "spectrum.hpp"
#include "util/scatteringutil.hpp"

namespace TK {
    BSDF::BSDF(const SurfaceInteraction& its, BxDF* bxdf) : m_Bxdf(bxdf) {
        m_N = its.n;
        m_T = its.dpdu;
        m_Bt = normalize(cross(m_N, m_T));
    }

    bool bxdfIsApplicable(BxDF* bxdf, bool sameHemisphere, BxDFType type) {
        return bxdf->matchesType(type) && ((sameHemisphere && (bxdf->m_Type & BXDF_REFLECTIVE)) ||
                                           (!sameHemisphere && (bxdf->m_Type & BXDF_TRANSMISSIVE)));
    }

    tkSpectrum BSDF::evaluate(const Vec3f& worldWo, const Vec3f& worldWi) const {
        Vec3f wo = worldToLocal(worldWo);
        Vec3f wi = worldToLocal(worldWi);
        if (wo.z == 0) // Ray incident parallel to surface
            return 0;

        return m_Bxdf->evaluate(wo, wi);
    }

    BSDFSample BSDF::sample(const Vec3f& worldWo, const Vec2f& u, BxDFType type) const {
        Vec3f wo = worldToLocal(worldWo);
        if (wo.z == 0)
            return {};

        BSDFSample ret = m_Bxdf->sample(wo, u, type);
        if (!ret)
            return {};

        ret.wi = localToWorld(ret.wi);
        return ret;
    }

    tkFloat BSDF::getPdf(const Vec3f& worldWo, const Vec3f& worldWi, BxDFType type) const {
        Vec3f wo = worldToLocal(worldWo);
        Vec3f wi = worldToLocal(worldWi);
        if (wo.z == 0)
            return 0;

        return m_Bxdf->getPdf(wo, wi, type);
    }

    Vec3f BSDF::worldToLocal(const Vec3f& v) const {
        return Vec3f(dot(m_T, v), dot(m_Bt, v), dot(m_N, v));
    }
    Vec3f BSDF::localToWorld(const Vec3f& v) const {
        return Vec3f(m_T.x * v.x + m_Bt.x * v.y + m_N.x * v.z, m_T.y * v.x + m_Bt.y * v.y + m_N.y * v.z,
                       m_T.z * v.x + m_Bt.z * v.y + m_N.z * v.z);
    }
}  // namespace TK
