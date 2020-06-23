#include "lambert.hpp"

namespace TK {
    tkSpectrum Lambert::operator()(const tkVec3f &wo, const tkVec3f &wi) const {
        return dhr * TK_INVPI;
    }
} // namespace TK
