#include "scattering.hpp"

#include "interaction.hpp"
#include "spectrum.hpp"
#include "util/scatteringutil.hpp"

namespace TK {
    Scattering::~Scattering() {
        for (int i = 0; i < numBxdf; ++i) {
            delete bxdfs[i];
        }
    }

    void Scattering::initialize(const SurfaceInteraction& interaction) {
        n = interaction.n;
        t = interaction.dpdu;
        bt = normalize(cross(n, t));
    }

    bool bxdfIsApplicable(BxDF* bxdf, bool sameHemisphere, BxDFType type) {
        return bxdf->matchesType(type) && ((sameHemisphere && (bxdf->type & BXDF_REFLECTIVE)) ||
                                           (!sameHemisphere && (bxdf->type & BXDF_TRANSMISSIVE)));
    }

    tkSpectrum Scattering::evaluate(const Vec3f& worldWo, const Vec3f& worldWi, BxDFType type) const {
        Vec3f wo = worldToLocal(worldWo);
        Vec3f wi = worldToLocal(worldWi);
        tkSpectrum ret;
        bool sameSide = isSameHemisphere(wo, wi);
        for (int i = 0; i < numBxdf; ++i) {
            if (bxdfIsApplicable(bxdfs[i], sameSide, type))
                ret += bxdfs[i]->evaluate(wo, wi);
        }
        return ret;
    }

    tkSpectrum Scattering::sample(const Vec3f& worldWo, Vec3f* worldWi, const Vec2f& samp, tkFloat* pdf,
                                  BxDFType* sampledType, BxDFType type) const {
        std::vector<BxDF*> matching;
        *pdf = 0;
        // Collect bxdfs that match the type criteria
        for (int i = 0; i < numBxdf; ++i) {
            if (bxdfs[i]->matchesType(type))
                matching.push_back(bxdfs[i]);
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
            *sampledType = sampleBxDF->type;

        // Compute pdf and bxdf values using matching bxdf contributions
        if (size > 1) {
            if (!(sampleBxDF->type & BXDF_SPECULAR)) {
                bool sameSide = isSameHemisphere(wo, wi);
                for (BxDF* b : matching) {
                    if (b != sampleBxDF) {
                        *pdf += b->getPdf(wo, wi);
                        if ((sameSide && (sampleBxDF->type & BXDF_REFLECTIVE)) ||
                            (!sameSide && (sampleBxDF->type & BXDF_TRANSMISSIVE)))
                            ret += b->evaluate(wo, wi);
                    }
                }
                *pdf /= size;
            }
        }

        return ret;
    }

    tkFloat Scattering::getPdf(const Vec3f& worldWo, const Vec3f& worldWi, BxDFType type) const {
        Vec3f wo = worldToLocal(worldWo);
        Vec3f wi = worldToLocal(worldWi);
        tkFloat ret = 0;
        int count = 0;
        bool sameSide = isSameHemisphere(wo, wi);
        for (int i = 0; i < numBxdf; ++i) {
            if (bxdfIsApplicable(bxdfs[i], sameSide, type)) {
                ret += bxdfs[i]->getPdf(wo, wi);
                count++;
            }
        }
        return count > 0 ? ret / count : 0;
    }

    bool Scattering::addContribution(BxDF* bxdf) {
        if (numBxdf == MAX_BXDF_COUNT)
            return false;

        bxdfs[numBxdf++] = bxdf;
        return true;
    }

    Vec3f Scattering::worldToLocal(const Vec3f& v) const {
        return Vec3f(dot(t, v), dot(bt, v), dot(n, v));
    }
    Vec3f Scattering::localToWorld(const Vec3f& v) const {
        return Vec3f(t.x * v.x + bt.x * v.y + n.x * v.z, t.y * v.x + bt.y * v.y + n.y * v.z,
                       t.z * v.x + bt.z * v.y + n.z * v.z);
    }
}  // namespace TK
