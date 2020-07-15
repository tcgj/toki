#include "scattering.hpp"

#include "bxdf.hpp"
#include "interaction/surfaceinteraction.hpp"
#include "spectrum/rgbspectrum.hpp"

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

    tkSpectrum Scattering::operator()(const tkVec3f &wo, const tkVec3f &wi) const {
        tkVec3f localWo = worldToLocal(wo);
        tkVec3f localWi = worldToLocal(wi);
        tkSpectrum bsdf;
        for (tkUInt i = 0; i < numBxdf; ++i) {
            bsdf += (*bxdfs[i])(localWo, localWi);
        }
        return bsdf;
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
