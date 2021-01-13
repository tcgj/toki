#include "bsdf.hpp"

#include "interaction.hpp"
#include "spectrum.hpp"
#include "util/scatteringutil.hpp"

namespace TK {
    BSDF::~BSDF() {
        for (int i = 0; i < m_NumBxdf; ++i) {
            delete m_Bxdfs[i];
        }
    }

    void BSDF::initialize(const SurfaceInteraction& its) {
        m_N = its.n;
        m_T = its.dpdu;
        m_Bt = normalize(cross(m_N, m_T));
    }

    bool bxdfIsApplicable(BxDF* bxdf, bool sameHemisphere, BxDFType type) {
        return bxdf->matchesType(type) && ((sameHemisphere && (bxdf->m_Type & BXDF_REFLECTIVE)) ||
                                           (!sameHemisphere && (bxdf->m_Type & BXDF_TRANSMISSIVE)));
    }

    tkSpectrum BSDF::evaluate(const Vec3f& worldWo, const Vec3f& worldWi, BxDFType type) const {
        Vec3f wo = worldToLocal(worldWo);
        Vec3f wi = worldToLocal(worldWi);
        tkSpectrum ret;
        bool sameSide = isSameHemisphere(wo, wi);
        for (int i = 0; i < m_NumBxdf; ++i) {
            if (bxdfIsApplicable(m_Bxdfs[i], sameSide, type))
                ret += m_Bxdfs[i]->evaluate(wo, wi);
        }
        return ret;
    }

    tkSpectrum BSDF::sample(const Vec3f& worldWo, Vec3f* worldWi, const Vec2f& samp, tkFloat* pdf,
                                  BxDFType* sampledType, BxDFType type) const {
        std::vector<BxDF*> matching;
        *pdf = 0;
        // Collect bxdfs that match the type criteria
        for (int i = 0; i < m_NumBxdf; ++i) {
            if (m_Bxdfs[i]->matchesType(type))
                matching.push_back(m_Bxdfs[i]);
        }
        int size = matching.size();
        if (size == 0)
            return 0;

        // Find bxdf to sample from
        int randIndex = std::min((int)std::floor(samp[0] * size), size - 1);
        BxDF* sampleBxDF = matching[randIndex];

        // Sample from selected bxdf
        Vec2f newSamp(samp[0] * size - randIndex, samp[1]);
        Vec3f wo = worldToLocal(worldWo);
        Vec3f wi;
        tkSpectrum ret = sampleBxDF->sample(wo, &wi, newSamp, pdf);
        if (*pdf == 0)
            return 0;

        *worldWi = localToWorld(wi);
        if (sampledType != nullptr)
            *sampledType = sampleBxDF->m_Type;

        // Compute pdf and bxdf values using matching bxdf contributions
        if (size > 1) {
            if (!(sampleBxDF->m_Type & BXDF_SPECULAR)) {
                bool sameSide = isSameHemisphere(wo, wi);
                for (BxDF* b : matching) {
                    if (b != sampleBxDF) {
                        *pdf += b->getPdf(wo, wi);
                        if ((sameSide && (sampleBxDF->m_Type & BXDF_REFLECTIVE)) ||
                            (!sameSide && (sampleBxDF->m_Type & BXDF_TRANSMISSIVE)))
                            ret += b->evaluate(wo, wi);
                    }
                }
                *pdf /= size;
            }
        }

        return ret;
    }

    tkFloat BSDF::getPdf(const Vec3f& worldWo, const Vec3f& worldWi, BxDFType type) const {
        Vec3f wo = worldToLocal(worldWo);
        Vec3f wi = worldToLocal(worldWi);
        tkFloat ret = 0;
        int count = 0;
        bool sameSide = isSameHemisphere(wo, wi);
        for (int i = 0; i < m_NumBxdf; ++i) {
            if (bxdfIsApplicable(m_Bxdfs[i], sameSide, type)) {
                ret += m_Bxdfs[i]->getPdf(wo, wi);
                count++;
            }
        }
        return count > 0 ? ret / count : 0;
    }

    bool BSDF::addContribution(BxDF* bxdf) {
        if (m_NumBxdf == MAX_BXDF_COUNT)
            return false;

        m_Bxdfs[m_NumBxdf++] = bxdf;
        return true;
    }

    Vec3f BSDF::worldToLocal(const Vec3f& v) const {
        return Vec3f(dot(m_T, v), dot(m_Bt, v), dot(m_N, v));
    }
    Vec3f BSDF::localToWorld(const Vec3f& v) const {
        return Vec3f(m_T.x * v.x + m_Bt.x * v.y + m_N.x * v.z, m_T.y * v.x + m_Bt.y * v.y + m_N.y * v.z,
                       m_T.z * v.x + m_Bt.z * v.y + m_N.z * v.z);
    }
}  // namespace TK
