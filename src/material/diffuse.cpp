#include "diffuse.hpp"

#include "core/bsdf.hpp"
#include "bsdf/lambert.hpp"

namespace TK {
    BSDF Diffuse::getBSDF(const SurfaceInteraction& its) const {
        return BSDF(its, Lambert(m_Kd));
    }
}  // namespace TK
