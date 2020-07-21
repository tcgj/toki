#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"

namespace TK {
    template <tkUInt numCoeff>
    class Spectrum {
    public:
        Spectrum(tkFloat f = 0.0f) {
            for (tkUInt i = 0; i < numCoeff; ++i) {
                c[i] = f;
            }
        }

        const Spectrum &operator+() const;
        Spectrum operator-() const;
        tkUInt operator[](tkInt i) const;
        tkUInt &operator[](tkInt i);

        Spectrum operator+(const Spectrum &s) const;
        Spectrum operator-(const Spectrum &s) const;
        Spectrum operator*(const Spectrum &s) const;
        Spectrum operator/(const Spectrum &s) const;
        Spectrum operator*(tkFloat f) const;
        Spectrum operator/(tkFloat f) const;

        Spectrum &operator+=(const Spectrum &s);
        Spectrum &operator-=(const Spectrum &s);
        Spectrum &operator*=(const Spectrum &s);
        Spectrum &operator/=(const Spectrum &s);
        Spectrum &operator*=(tkFloat f);
        Spectrum &operator/=(tkFloat f);

        bool operator==(const Spectrum &s) const;
        bool operator!=(const Spectrum &s) const;

        friend Spectrum operator*(tkFloat f, const Spectrum &s) {
            Spectrum ret = s;
            for (tkUInt i = 0; i < numCoeff; ++i)
                ret.c[i] *= f;

            return ret;
        }

        bool isBlack() const;
        friend Spectrum sqrt(const Spectrum &s);
        friend Spectrum pow(const Spectrum &s, tkFloat p);
        friend Spectrum exp(const Spectrum &s);
        friend Spectrum clamp(const Spectrum &s, tkFloat lo, tkFloat hi);

    protected:
        tkFloat c[numCoeff];
    };

    template <tkUInt nC>
    inline bool isNaN(Spectrum<nC> &s) {
        for (tkUInt i = 0; i < nC; ++i) {
            if (std::isnan(s.c[i]))
                return true;
        }
        return false;
    }

    template <tkUInt nC>
    inline const Spectrum<nC> &Spectrum<nC>::operator+() const {
        return *this;
    }
    template <tkUInt nC>
    inline Spectrum<nC> Spectrum<nC>::operator-() const {
        Spectrum<nC> ret;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] = -c[i];

        return ret;
    }
    template <tkUInt nC>
    inline tkUInt Spectrum<nC>::operator[](tkInt i) const {
        tkAssert(i >= 0 && i < nC);
        return c[i];
    }
    template <tkUInt nC>
    inline tkUInt &Spectrum<nC>::operator[](tkInt i) {
        tkAssert(i >= 0 && i < nC);
        return c[i];
    }

    template <tkUInt nC>
    inline Spectrum<nC> Spectrum<nC>::operator+(const Spectrum<nC> &s) const {
        Spectrum<nC> ret = *this;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] += s.c[i];

        return ret;
    }
    template <tkUInt nC>
    inline Spectrum<nC> Spectrum<nC>::operator-(const Spectrum<nC> &s) const {
        Spectrum<nC> ret = *this;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] -= s.c[i];

        return ret;
    }
    template <tkUInt nC>
    inline Spectrum<nC> Spectrum<nC>::operator*(const Spectrum<nC> &s) const {
        Spectrum<nC> ret = *this;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] *= s.c[i];

        return ret;
    }
    template <tkUInt nC>
    inline Spectrum<nC> Spectrum<nC>::operator/(const Spectrum<nC> &s) const {
        tkAssert(!isNaN(s));
        Spectrum<nC> ret = *this;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] /= s.c[i];

        return ret;
    }
    template <tkUInt nC>
    inline Spectrum<nC> Spectrum<nC>::operator*(tkFloat f) const {
        Spectrum<nC> ret = *this;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] *= f;

        return ret;
    }

    template <tkUInt nC>
    inline Spectrum<nC> Spectrum<nC>::operator/(tkFloat f) const {
        tkAssert(f != 0);
        Spectrum<nC> ret = *this;
        tkFloat invF = 1.0 / f;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] *= invF;

        return ret;
    }

    template <tkUInt nC>
    inline Spectrum<nC> &Spectrum<nC>::operator+=(const Spectrum<nC> &s) {
        for (tkUInt i = 0; i < nC; ++i)
            c[i] += s.c[i];

        return *this;
    }
    template <tkUInt nC>
    inline Spectrum<nC> &Spectrum<nC>::operator-=(const Spectrum<nC> &s) {
        for (tkUInt i = 0; i < nC; ++i)
            c[i] -= s.c[i];

        return *this;
    }
    template <tkUInt nC>
    inline Spectrum<nC> &Spectrum<nC>::operator*=(const Spectrum<nC> &s) {
        for (tkUInt i = 0; i < nC; ++i)
            c[i] *= s.c[i];

        return *this;
    }
    template <tkUInt nC>
    inline Spectrum<nC> &Spectrum<nC>::operator/=(const Spectrum<nC> &s) {
        tkAssert(!isNaN(s));
        for (tkUInt i = 0; i < nC; ++i)
            c[i] /= s.c[i];

        return *this;
    }
    template <tkUInt nC>
    inline Spectrum<nC> &Spectrum<nC>::operator*=(tkFloat f) {
        for (tkUInt i = 0; i < nC; ++i)
            c[i] *= f;

        return *this;
    }
    template <tkUInt nC>
    inline Spectrum<nC> &Spectrum<nC>::operator/=(tkFloat f) {
        tkAssert(f != 0);
        tkFloat invF = 1.0 / f;
        for (tkUInt i = 0; i < nC; ++i)
            c[i] *= invF;

        return *this;
    }

    template <tkUInt nC>
    inline bool Spectrum<nC>::operator==(const Spectrum<nC> &s) const {
        for (tkUInt i = 0; i < nC; ++i)
            if (c[i] != s.c[i])
                return false;

        return true;
    }
    template <tkUInt nC>
    inline bool Spectrum<nC>::operator!=(const Spectrum<nC> &s) const {
        for (tkUInt i = 0; i < nC; ++i)
            if (c[i] == s.c[i])
                return false;

        return true;
    }

    // Spectrum operations
    template <tkUInt nC>
    inline bool Spectrum<nC>::isBlack() const {
        for (tkUInt i = 0; i < nC; ++i)
            if (c[i] != 0.0)
                return false;

        return true;
    }
    template <tkUInt nC>
    inline Spectrum<nC> sqrt(const Spectrum<nC> &s) {
        Spectrum<nC> ret;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] = std::sqrt(s.c[i]);

        return ret;
    }
    template <tkUInt nC>
    inline Spectrum<nC> pow(const Spectrum<nC> &s, tkFloat p) {
        Spectrum<nC> ret;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] = std::pow(s.c[i], p);

        return ret;
    }
    template <tkUInt nC>
    inline Spectrum<nC> exp(const Spectrum<nC> &s) {
        Spectrum<nC> ret;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] = std::exp(s.c[i]);

        return ret;
    }

    template <tkUInt nC>
    inline Spectrum<nC> clamp(const Spectrum<nC> &s, tkFloat lo = 0, tkFloat hi = TK_INFINITY) {
        Spectrum<nC> ret;
        for (tkUInt i = 0; i < nC; ++i)
            ret.c[i] = clamp(s.c[i], lo, hi);

        return ret;
    }


    class RGBSpectrum : public Spectrum<3> {
    public:
        RGBSpectrum(tkFloat f = 0.0f) : Spectrum(f) {}
        RGBSpectrum(const Spectrum<3> &s) : Spectrum(s) {}

        tkVec3f toRGB() const;
        static RGBSpectrum fromRGB(const tkVec3f &rgb);
    };

    inline tkVec3f RGBSpectrum::toRGB() const {
        return tkVec3f(c[0], c[1], c[2]);
    }

    inline RGBSpectrum RGBSpectrum::fromRGB(const tkVec3f &rgb) {
        RGBSpectrum ret;
        ret.c[0] = rgb.r;
        ret.c[1] = rgb.g;
        ret.c[2] = rgb.b;
        return ret;
    }
} // namespace TK