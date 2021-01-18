#pragma once

#include "core/material.hpp"
#include "core/spectrum.hpp"

namespace TK {
    class Diffuse : public Material {
    public:
        Diffuse(const tkSpectrum& kd) : m_Kd(kd) {}

        BSDF getBSDF(const SurfaceInteraction& its) const override;

    private:
        tkSpectrum m_Kd;
    };
}  // namespace TK
