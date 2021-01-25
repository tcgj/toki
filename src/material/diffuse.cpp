#include "diffuse.hpp"

#include "core/bsdf.hpp"

namespace TK {
    BSDF Diffuse::getBSDF(const SurfaceInteraction& its) const {
        return BSDF(its, new Lambert(m_Kd));
    }
}  // namespace TK
