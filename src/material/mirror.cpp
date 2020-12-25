#include "mirror.hpp"

#include "core/bsdf.hpp"
#include "core/fresnel.hpp"
#include "bsdf/specularreflection.hpp"

namespace TK {
    void Mirror::computeScattering(BSDF* bsdf) const {
        if (!m_Kr.isBlack())
            bsdf->addContribution(new SpecularReflection(m_Kr, new NoOpFresnel()));
    }
}  // namespace TK
