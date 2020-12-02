#include "lambert.hpp"

namespace TK {
    tkSpectrum Lambert::evaluate(const tkVec3f& wo, const tkVec3f& wi) const {
        return dhr * TK_INVPI;
    }
}  // namespace TK
