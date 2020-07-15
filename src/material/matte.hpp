#pragma once

#include "core/material.hpp"
#include "core/spectrum.hpp"

namespace TK {
    class Matte : public Material {
    public:
        Matte(const tkSpectrum &kd) : kd(kd) {}

        void computeScattering(Scattering *scattering) const override;

    private:
        tkSpectrum kd;
    };
} // namespace TK
