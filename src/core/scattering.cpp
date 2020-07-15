#include "scattering.hpp"

#include <vector>

#include "interaction.hpp"
#include "spectrum.hpp"
#include "util/scatteringutil.hpp"

namespace TK {
    Scattering::Scattering(const SurfaceInteraction &interaction) {
        n = normalize(interaction.n);
        t = normalize(interaction.t);
        bt = normalize(cross(n, t));
    }

    Scattering::~Scattering() {
        for (tkUInt i = 0; i < numBxdf; ++i) {
            delete bxdfs[i];
        }
    }

    tkSpectrum Scattering::evaluate(const tkVec3f &worldWo, const tkVec3f &worldWi) const {
        tkVec3f wo = worldToLocal(worldWo);
        tkVec3f wi = worldToLocal(worldWi);
        tkSpectrum ret;
        for (tkUInt i = 0; i < numBxdf; ++i) {
            ret += bxdfs[i]->evaluate(wo, wi);
        }
        return ret;
    }

    tkSpectrum Scattering::sample(const tkVec3f &worldWo, tkVec3f *worldWi,
                                  const tkVec2f &samp, tkFloat *pdf,
                                  BxDFType type) const {
        std::vector<BxDF *> matching;
        *pdf = 0;
        // Collect bxdfs that match the type criteria
        for (tkUInt i = 0; i < numBxdf; ++i) {
            if (bxdfs[i]->hasType(type))
                matching.push_back(bxdfs[i]);
        }
        tkInt size = matching.size();
        if (size == 0)
            return 0;

        // Find bxdf to sample from
        tkInt randIndex = std::min((tkInt)std::floor(samp[0] * size), size - 1);
        BxDF *sampleBxDF = matching[randIndex];

        // Sample from selected bxdf
        tkVec2f newSamp(samp[0] * size - randIndex, samp[1]);
        tkVec3f wo = worldToLocal(worldWo), wi;
        tkSpectrum ret = sampleBxDF->sample(wo, &wi, newSamp, pdf);
        if (pdf == 0)
            return 0;
        *worldWi = localToWorld(wi);

        // Compute pdf and bxdf values using matching bxdf contributions
        if (size > 1) {
            if (!sampleBxDF->hasType(BXDF_SPECULAR)) {
                bool sameSide = isSameHemisphere(wo, wi);
                for (BxDF *b : matching) {
                    if (b != sampleBxDF) {
                        *pdf += b->getPdf(wo, wi);
                        if ((sameSide && sampleBxDF->hasType(BXDF_REFLECTIVE)) ||
                            (!sameSide && sampleBxDF->hasType(BXDF_TRANSMISSIVE)))
                            ret += b->evaluate(wo, wi);
                    }
                }
            }
            *pdf /= size;
        }

        return ret;
    }

    tkFloat Scattering::getPdf(const tkVec3f &worldWo, const tkVec3f &worldWi,
                   BxDFType type = BXDF_ALL) const {
        tkVec3f wo = worldToLocal(worldWo);
        tkVec3f wi = worldToLocal(worldWi);
        tkFloat ret;
        tkInt count;
        for (tkUInt i = 0; i < numBxdf; ++i) {
            if (bxdfs[i]->hasType(type)) {
                ret += bxdfs[i]->getPdf(wo, wi);
                count++;
            }
        }
        return count > 0 ? ret / count : 0;
    }

    bool Scattering::addContribution(BxDF *bxdf) {
        if (numBxdf == MAX_BXDF_COUNT)
            return false;

        bxdfs[numBxdf++] = bxdf;
        return true;
    }

    tkVec3f Scattering::worldToLocal(const tkVec3f &v) const {
        return tkVec3f(dot(t, v), dot(bt, v), dot(n, v));
    }
    tkVec3f Scattering::localToWorld(const tkVec3f &v) const {
        return tkVec3f(t.x * v.x + bt.x * v.y + n.x * v.z,
                       t.y * v.x + bt.y * v.y + n.y * v.z,
                       t.z * v.x + bt.z * v.y + n.z * v.z);
    }
} // namespace TK
