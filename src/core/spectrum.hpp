#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"

namespace TK {
    template <int NCoefficients>
    class Spectrum {
    public:
        Spectrum(tkFloat f = 0.0f) {
            for (int i = 0; i < NCoefficients; ++i) {
                c[i] = f;
            }
        }

        const Spectrum& operator+() const;
        Spectrum operator-() const;
        tkFloat operator[](int i) const;
        tkFloat& operator[](int i);

        Spectrum operator+(const Spectrum& s) const;
        Spectrum operator-(const Spectrum& s) const;
        Spectrum operator*(const Spectrum& s) const;
        Spectrum operator/(const Spectrum& s) const;
        Spectrum operator*(tkFloat f) const;
        Spectrum operator/(tkFloat f) const;

        Spectrum& operator+=(const Spectrum& s);
        Spectrum& operator-=(const Spectrum& s);
        Spectrum& operator*=(const Spectrum& s);
        Spectrum& operator/=(const Spectrum& s);
        Spectrum& operator*=(tkFloat f);
        Spectrum& operator/=(tkFloat f);

        bool operator==(const Spectrum& s) const;
        bool operator!=(const Spectrum& s) const;

        bool isBlack() const;

        friend Spectrum operator*(tkFloat f, const Spectrum& s) {
            Spectrum ret = s;
            for (int i = 0; i < NCoefficients; ++i)
                ret.c[i] *= f;

            return ret;
        }
        inline Spectrum sqrt(const Spectrum& s) {
            Spectrum ret;
            for (int i = 0; i < NCoefficients; ++i)
                ret.c[i] = std::sqrt(s.c[i]);

            return ret;
        }
        friend Spectrum pow(const Spectrum& s, tkFloat p) {
            Spectrum ret;
            for (int i = 0; i < NCoefficients; ++i)
                ret.c[i] = std::pow(s.c[i], p);

            return ret;
        }
        friend Spectrum exp(const Spectrum& s) {
            Spectrum ret;
            for (int i = 0; i < NCoefficients; ++i)
                ret.c[i] = std::exp(s.c[i]);

            return ret;
        }

    protected:
        tkFloat c[NCoefficients];
    };

    template <int N>
    inline bool isNaN(Spectrum<N>& s) {
        for (int i = 0; i < N; ++i) {
            if (std::isnan(s.c[i]))
                return true;
        }
        return false;
    }

    template <int N>
    inline const Spectrum<N>& Spectrum<N>::operator+() const {
        return *this;
    }
    template <int N>
    inline Spectrum<N> Spectrum<N>::operator-() const {
        Spectrum<N> ret;
        for (int i = 0; i < N; ++i)
            ret.c[i] = -c[i];

        return ret;
    }
    template <int N>
    inline tkFloat Spectrum<N>::operator[](int i) const {
        tkAssert(i >= 0 && i < N);
        return c[i];
    }
    template <int N>
    inline tkFloat& Spectrum<N>::operator[](int i) {
        tkAssert(i >= 0 && i < N);
        return c[i];
    }

    template <int N>
    inline Spectrum<N> Spectrum<N>::operator+(const Spectrum<N>& s) const {
        Spectrum<N> ret = *this;
        for (int i = 0; i < N; ++i)
            ret.c[i] += s.c[i];

        return ret;
    }
    template <int N>
    inline Spectrum<N> Spectrum<N>::operator-(const Spectrum<N>& s) const {
        Spectrum<N> ret = *this;
        for (int i = 0; i < N; ++i)
            ret.c[i] -= s.c[i];

        return ret;
    }
    template <int N>
    inline Spectrum<N> Spectrum<N>::operator*(const Spectrum<N>& s) const {
        Spectrum<N> ret = *this;
        for (int i = 0; i < N; ++i)
            ret.c[i] *= s.c[i];

        return ret;
    }
    template <int N>
    inline Spectrum<N> Spectrum<N>::operator/(const Spectrum<N>& s) const {
        tkAssert(!isNaN(s));
        Spectrum<N> ret = *this;
        for (int i = 0; i < N; ++i)
            ret.c[i] /= s.c[i];

        return ret;
    }
    template <int N>
    inline Spectrum<N> Spectrum<N>::operator*(tkFloat f) const {
        Spectrum<N> ret = *this;
        for (int i = 0; i < N; ++i)
            ret.c[i] *= f;

        return ret;
    }

    template <int N>
    inline Spectrum<N> Spectrum<N>::operator/(tkFloat f) const {
        tkAssert(f != 0);
        Spectrum<N> ret = *this;
        tkFloat invF = 1.0 / f;
        for (int i = 0; i < N; ++i)
            ret.c[i] *= invF;

        return ret;
    }

    template <int N>
    inline Spectrum<N>& Spectrum<N>::operator+=(const Spectrum<N>& s) {
        for (int i = 0; i < N; ++i)
            c[i] += s.c[i];

        return *this;
    }
    template <int N>
    inline Spectrum<N>& Spectrum<N>::operator-=(const Spectrum<N>& s) {
        for (int i = 0; i < N; ++i)
            c[i] -= s.c[i];

        return *this;
    }
    template <int N>
    inline Spectrum<N>& Spectrum<N>::operator*=(const Spectrum<N>& s) {
        for (int i = 0; i < N; ++i)
            c[i] *= s.c[i];

        return *this;
    }
    template <int N>
    inline Spectrum<N>& Spectrum<N>::operator/=(const Spectrum<N>& s) {
        tkAssert(!isNaN(s));
        for (int i = 0; i < N; ++i)
            c[i] /= s.c[i];

        return *this;
    }
    template <int N>
    inline Spectrum<N>& Spectrum<N>::operator*=(tkFloat f) {
        for (int i = 0; i < N; ++i)
            c[i] *= f;

        return *this;
    }
    template <int N>
    inline Spectrum<N>& Spectrum<N>::operator/=(tkFloat f) {
        tkAssert(f != 0);
        tkFloat invF = 1.0 / f;
        for (int i = 0; i < N; ++i)
            c[i] *= invF;

        return *this;
    }

    template <int N>
    inline bool Spectrum<N>::operator==(const Spectrum<N>& s) const {
        for (int i = 0; i < N; ++i)
            if (c[i] != s.c[i])
                return false;

        return true;
    }
    template <int N>
    inline bool Spectrum<N>::operator!=(const Spectrum<N>& s) const {
        for (int i = 0; i < N; ++i)
            if (c[i] == s.c[i])
                return false;

        return true;
    }

    // Spectrum operations
    template <int N>
    inline bool Spectrum<N>::isBlack() const {
        for (int i = 0; i < N; ++i)
            if (c[i] != 0.0)
                return false;

        return true;
    }

    class RGBSpectrum : public Spectrum<3> {
    public:
        RGBSpectrum(tkFloat f = 0.0f) : Spectrum(f) {}
        RGBSpectrum(const Spectrum<3>& s) : Spectrum(s) {}

        tkFloat luminance() const;
        Vec3f toRGB() const;
        static RGBSpectrum fromRGB(const Vec3f& rgb);
    };

    inline tkFloat RGBSpectrum::luminance() const {
        return 0.2126 * c[0] + 0.7152 * c[1] + 0.0722 * c[2];
    }

    inline Vec3f RGBSpectrum::toRGB() const {
        return Vec3f(c[0], c[1], c[2]);
    }

    inline RGBSpectrum RGBSpectrum::fromRGB(const Vec3f& rgb) {
        RGBSpectrum ret;
        ret.c[0] = rgb.r;
        ret.c[1] = rgb.g;
        ret.c[2] = rgb.b;
        return ret;
    }
}  // namespace TK