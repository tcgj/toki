#include "mirror.hpp"

#include "core/scattering.hpp"
#include "core/fresnel.hpp"
#include "bsdf/specularreflection.hpp"

namespace TK {
    void Mirror::computeScattering(Scattering* scattering) const {
        if (!kr.isBlack())
            scattering->addContribution(new SpecularReflection(kr, new NoOpFresnel()));
    }
}  // namespace TK
