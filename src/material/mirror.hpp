#pragma once

#include "core/material.hpp"
#include "core/spectrum.hpp"

namespace TK {
    class Mirror : public Material {
    public:
        Mirror(const tkSpectrum& kr) : kr(kr) {}

        void computeScattering(BSDF* bsdf) const override;

    private:
        tkSpectrum kr;
    };
}  // namespace TK
