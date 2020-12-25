#pragma once

#include "core/material.hpp"
#include "core/spectrum.hpp"

namespace TK {
    class Mirror : public Material {
    public:
        Mirror(const tkSpectrum& kr) : m_Kr(kr) {}

        void computeScattering(BSDF* bsdf) const override;

    private:
        tkSpectrum m_Kr;
    };
}  // namespace TK
