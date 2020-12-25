#pragma once

#include "core/material.hpp"
#include "core/spectrum.hpp"

namespace TK {
    class Matte : public Material {
    public:
        Matte(const tkSpectrum& kd) : m_Kd(kd) {}

        void computeScattering(BSDF* bsdf) const override;

    private:
        tkSpectrum m_Kd;
    };
}  // namespace TK
