#pragma once

#include "system/system.hpp"
#include "spectrum/spectrum.hpp"
#include "spectrum/rgbspectrum.hpp"

namespace TK {
    tkFloat ComputeDielectricFresnel(tkFloat cosI, tkFloat etaI,
                                     tkFloat etaT);
    tkSpectrum ComputeConductorFresnel(tkFloat cosI, const tkSpectrum &eta,
                                       const tkSpectrum &etaK);

    class Fresnel {
    public:
        virtual tkSpectrum evaluate(tkFloat cosI) const = 0;
    };

    class ConductorFresnel : public Fresnel {
    public:
        ConductorFresnel(const tkSpectrum &eta, const tkSpectrum &etaK)
            : eta(eta), etaK(etaK) {}

        tkSpectrum evaluate(tkFloat cosI) const override;
    private:
        tkSpectrum eta, etaK;
    };

    class DielectricFresnel : public Fresnel {
    public:
        DielectricFresnel(tkFloat etaA, tkFloat etaB)
            : etaA(etaA), etaB(etaB) {}

        tkSpectrum evaluate(tkFloat cosI) const override;

    private:
        tkFloat etaA, etaB;
    };

    class NoOpFresnel : public Fresnel {
    public:
        tkSpectrum evaluate(tkFloat cosI) const override;
    };
}  // namespace TK
