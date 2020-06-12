#include "bxdf.hpp"

#include "spectrum/rgbspectrum.hpp"

namespace TK {
    bool BxDF::hasType(BxDFType t) const {
        return (t & type) == type;
    }

    tkSpectrum BxDF::getSample(const tkVec3f &wo, tkVec3f *wi,
                                 tkFloat *pdf) const {
        
    }
} // namespace TK
