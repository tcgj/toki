#pragma once

#include "math/math.hpp"

namespace TK {
    struct Matrix44 {
        Matrix44();
        Matrix44(tkFloat d[16]);
        Matrix44(tkFloat f00, tkFloat f01, tkFloat f02, tkFloat f03, tkFloat f10,
                tkFloat f11, tkFloat f12, tkFloat f13, tkFloat f20, tkFloat f21,
                tkFloat f22, tkFloat f23, tkFloat f30, tkFloat f31, tkFloat f32,
                tkFloat f33);

        //  Assignment operators
        Matrix44 &operator*=(const Matrix44 &m);

        // Equality
        bool operator==(const Matrix44 &m) const;
        bool operator!=(const Matrix44 &m) const;

        tkFloat determinant() const;

        friend Matrix44 transpose(const Matrix44 &m);
        friend Matrix44 inverse(const Matrix44 &m);

        tkFloat data[16];
    };

    Matrix44 operator*(const Matrix44 &m1, const Matrix44 &m2);
    std::ostream &operator<<(std::ostream &os, const Matrix44 &m);
} // namespace TK
