#include "matte.hpp"

#include "core/bsdf.hpp"
#include "bsdf/lambert.hpp"

namespace TK {
    void Matte::computeScattering(BSDF* bsdf) const {
        if (!m_Kd.isBlack())
            bsdf->addContribution(new Lambert(m_Kd));
    }
}  // namespace TK
