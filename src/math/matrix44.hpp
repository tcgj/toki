#pragma once

#include "system/toki.hpp"

namespace TK {
    class Matrix44 {
    public:
        Matrix44();
        Matrix44(tkFloat d[16]);
        Matrix44(tkFloat f00, tkFloat f01, tkFloat f02, tkFloat f03, tkFloat f10, tkFloat f11, tkFloat f12,
                 tkFloat f13, tkFloat f20, tkFloat f21, tkFloat f22, tkFloat f23, tkFloat f30, tkFloat f31,
                 tkFloat f32, tkFloat f33);

        //  Assignment operators
        Matrix44& operator*=(const Matrix44& m);

        // Equality
        bool operator==(const Matrix44& m) const;
        bool operator!=(const Matrix44& m) const;

        tkFloat determinant() const;

        std::string toString() const;

        friend Matrix44 transpose(const Matrix44& m);
        friend Matrix44 inverse(const Matrix44& m);

        tkFloat m_Entries[16];
    };

    Matrix44 operator*(const Matrix44& m1, const Matrix44& m2);
    std::istream& operator>>(std::istream& is, Matrix44& m);
    std::ostream& operator<<(std::ostream& os, const Matrix44& m);
}  // namespace TK
