#include "matte.hpp"

#include "core/scatter.hpp"
#include "scatter/lambert.hpp"

namespace TK {
    void Matte::updateScatter(Scatter *scatter) const {
        if (!kd.isBlack())
            scatter->addContribution(new Lambert(kd));
    }
} // namespace TK