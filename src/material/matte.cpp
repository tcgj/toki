#include "matte.hpp"

#include "core/bsdf.hpp"
#include "bsdf/lambert.hpp"

namespace TK {
    void Matte::computeScattering(BSDF* bsdf) const {
        if (!kd.isBlack())
            bsdf->addContribution(new Lambert(kd));
    }
}  // namespace TK
