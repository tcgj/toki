#include "fresnel.hpp"

namespace TK {
    tkFloat dielectricFresnel(tkFloat cosI, tkFloat etaA,
                                     tkFloat etaB) {
        bool entering = cosI > 0;
        tkFloat etaI = entering ? etaA : etaB;
        tkFloat etaT = entering ? etaB : etaA;
        if (!entering)
            cosI = std::abs(cosI);

        tkFloat sinI = std::sqrt(std::max((tkFloat)0, 1 - cosI * cosI));
        tkFloat sinT = etaI * sinI / etaT;

        if (sinT >= 1) {
            return 1;
        }
        tkFloat cosT = std::sqrt(std::max((tkFloat)0, 1 - sinT * sinT));

        tkFloat rParallel = ((etaT * cosI) - (etaI * cosT)) /
                            ((etaT * cosI) + (etaI * cosT));
        tkFloat rPerp = ((etaI * cosI) - (etaT * cosT)) /
                        ((etaI * cosI) + (etaT * cosT));
        return (rParallel * rParallel + rPerp * rPerp) * 0.5;
    }

    tkSpectrum conductorFresnel(tkFloat cosI, const tkSpectrum &eta,
                                       const tkSpectrum &etaK) {
        // Only an approximation of air-conductor fresnel
        // Referenced from https://seblagarde.wordpress.com/2013/04/29/memo-on-fresnel-equations/
        tkFloat cosISqr = cosI * cosI;
        tkSpectrum twoEtaCosI = 2 * eta * cosI;
        tkSpectrum t0 = eta * eta + etaK * etaK;
        tkSpectrum t1 = t0 * cosISqr;
        tkSpectrum rS = (t0 - twoEtaCosI + cosISqr) / (t0 + twoEtaCosI + cosISqr);
        tkSpectrum rP = (t1 - twoEtaCosI + 1) / (t1 + twoEtaCosI + 1);
        return (rP + rS) * 0.5;
    }

    tkSpectrum ConductorFresnel::evaluate(tkFloat cosI) const {
        return conductorFresnel(cosI, eta, etaK);
    }

    tkSpectrum DielectricFresnel::evaluate(tkFloat cosI) const {
        return dielectricFresnel(cosI, etaA, etaB);
    }

    tkSpectrum NoOpFresnel::evaluate(tkFloat cosI) const {
        return 1;
    }
}  // namespace TK
