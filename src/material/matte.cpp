#include "matte.hpp"

#include "core/scattering.hpp"
#include "scattering/lambert.hpp"

namespace TK {
    void Matte::computeScattering(Scattering *scattering) const {
        if (!kd.isBlack())
            scattering->addContribution(new Lambert(kd));
    }
} // namespace TK
