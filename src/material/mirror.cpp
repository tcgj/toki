#include "mirror.hpp"

#include "core/bsdf.hpp"
#include "core/fresnel.hpp"
#include "bsdf/specularreflection.hpp"

namespace TK {
    void Mirror::computeScattering(BSDF* bsdf) const {
        if (!kr.isBlack())
            bsdf->addContribution(new SpecularReflection(kr, new NoOpFresnel()));
    }
}  // namespace TK
