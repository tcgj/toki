#include "lambert.hpp"

namespace TK {
    tkSpectrum Lambert::evaluate(const Vec3f& wo, const Vec3f& wi) const {
        return m_Dhr * TK_INVPI;
    }
}  // namespace TK
