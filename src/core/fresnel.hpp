#pragma once

#include "system/toki.hpp"
#include "spectrum.hpp"

namespace TK {
    tkFloat dielectricFresnel(tkFloat cosI, tkFloat etaI, tkFloat etaT);

    tkSpectrum conductorFresnel(tkFloat cosI, const tkSpectrum& eta, const tkSpectrum& etaK);

    class Fresnel {
    public:
        virtual tkSpectrum evaluate(tkFloat cosI) const = 0;
    };

    class ConductorFresnel : public Fresnel {
    public:
        ConductorFresnel(const tkSpectrum& eta, const tkSpectrum& etaK) : m_Eta(eta), m_EtaK(etaK) {}

        tkSpectrum evaluate(tkFloat cosI) const override;

    private:
        tkSpectrum m_Eta;
        tkSpectrum m_EtaK;
    };

    class DielectricFresnel : public Fresnel {
    public:
        DielectricFresnel(tkFloat etaA, tkFloat etaB) : m_EtaA(etaA), m_EtaB(etaB) {}

        tkSpectrum evaluate(tkFloat cosI) const override;

    private:
        tkFloat m_EtaA;
        tkFloat m_EtaB;
    };

    class NoOpFresnel : public Fresnel {
    public:
        tkSpectrum evaluate(tkFloat cosI) const override;
    };
}  // namespace TK
