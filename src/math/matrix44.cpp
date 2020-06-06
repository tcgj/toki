#include "matrix44.hpp"

#include <ostream>
#include "math/math.hpp"

namespace TK {
    Matrix44::Matrix44() {
        data[0]  = 1.0f; data[1]  = 0.0f; data[2]  = 0.0f; data[3]  = 0.0f;
        data[4]  = 0.0f; data[5]  = 1.0f; data[6]  = 0.0f; data[7]  = 0.0f;
        data[8]  = 0.0f; data[9]  = 0.0f; data[10] = 1.0f; data[11] = 0.0f;
        data[12] = 0.0f; data[13] = 0.0f; data[14] = 0.0f; data[15] = 1.0f;
    }
    Matrix44::Matrix44(tkFloat d[16]) {
        memcpy(data, d, 16 * sizeof(tkFloat));
    }
    Matrix44::Matrix44(tkFloat f00, tkFloat f01, tkFloat f02, tkFloat f03,
                    tkFloat f10, tkFloat f11, tkFloat f12, tkFloat f13,
                    tkFloat f20, tkFloat f21, tkFloat f22, tkFloat f23,
                    tkFloat f30, tkFloat f31, tkFloat f32, tkFloat f33) {
        data[0] = f00;
        data[1] = f01;
        data[2] = f02;
        data[3] = f03;
        data[4] = f10;
        data[5] = f11;
        data[6] = f12;
        data[7] = f13;
        data[8] = f20;
        data[9] = f21;
        data[10] = f22;
        data[11] = f23;
        data[12] = f30;
        data[13] = f31;
        data[14] = f32;
        data[15] = f33;
    }

    Matrix44 &Matrix44::operator*=(const Matrix44 &m) {
        for (tkUInt i = 0; i < 4; ++i) {
            for (tkUInt j = 0; j < 4; ++j) {
                data[i * 4 + j] = data[i * 4] * m.data[j] +
                                data[i * 4 + 1] * m.data[4 + j] +
                                data[i * 4 + 2] * m.data[8 + j] +
                                data[i * 4 + 3] * m.data[12 + j];
            }
        }
        return *this;
    }
    bool Matrix44::operator==(const Matrix44 &m) const {
        for (tkUInt i = 0; i < 16; ++i)
            if (data[i] != m.data[i]) return false;
        return true;
    }
    bool Matrix44::operator!=(const Matrix44 &m) const {
        for (tkUInt i = 0; i < 16; ++i)
            if (data[i] != m.data[i]) return true;
        return false;
    }

    // Binary operators
    Matrix44 operator*(const Matrix44 &m1, const Matrix44 &m2) {
        Matrix44 m;
        for (tkUInt i = 0; i < 4; ++i) {
            for (tkUInt j = 0; j < 4; ++j) {
                m.data[i * 4 + j] =
                    m1.data[i * 4] * m2.data[j] + m1.data[i * 4 + 1] * m2.data[4 + j] +
                    m1.data[i * 4 + 2] * m2.data[8 + j] + m1.data[i * 4 + 3] * m2.data[12 + j];
            }
        }
        return m;
    }

    // Matrix operations
    tkFloat Matrix44::determinant() const {
        return (data[0] * data[5] - data[1] * data[4]) *
                (data[10] * data[15] - data[11] * data[14]) -
            (data[0] * data[6] - data[2] * data[4]) *
                (data[9] * data[15] - data[11] * data[13]) +
            (data[0] * data[7] - data[3] * data[4]) *
                (data[9] * data[14] - data[10] * data[13]) +
            (data[1] * data[6] - data[2] * data[5]) *
                (data[8] * data[15] - data[11] * data[12]) -
            (data[1] * data[7] - data[3] * data[5]) *
                (data[8] * data[14] - data[10] * data[12]) +
            (data[2] * data[7] - data[3] * data[6]) *
                (data[8] * data[13] - data[9] * data[12]);
    }

    Matrix44 transpose(const Matrix44 &m) {
        return Matrix44(m.data[0], m.data[4], m.data[8], m.data[12],
                        m.data[1], m.data[5], m.data[9], m.data[13],
                        m.data[2], m.data[6], m.data[10], m.data[14],
                        m.data[3], m.data[7], m.data[11], m.data[15]);
    }
    Matrix44 inverse(const Matrix44 &m) {
        tkFloat d = m.determinant();
        if (std::abs(d) < TK_EPSILON)
            return Matrix44();

        // use Cramer's rule
        d = 1.0 / d;
        Matrix44 result;
        result.data[0] =
            d * (m.data[5] * (m.data[10] * m.data[15] - m.data[11] * m.data[14]) +
                m.data[6] * (m.data[11] * m.data[13] - m.data[9] * m.data[15]) +
                m.data[7] * (m.data[9] * m.data[14] - m.data[10] * m.data[13]));
        result.data[1] =
            d * (m.data[9] * (m.data[2] * m.data[15] - m.data[3] * m.data[14]) +
                m.data[10] * (m.data[3] * m.data[13] - m.data[1] * m.data[15]) +
                m.data[11] * (m.data[1] * m.data[14] - m.data[2] * m.data[13]));
        result.data[2] =
            d * (m.data[13] * (m.data[2] * m.data[7] - m.data[3] * m.data[6]) +
                m.data[14] * (m.data[3] * m.data[5] - m.data[1] * m.data[7]) +
                m.data[15] * (m.data[1] * m.data[6] - m.data[2] * m.data[5]));
        result.data[3] =
            d * (m.data[1] * (m.data[7] * m.data[10] - m.data[6] * m.data[11]) +
                m.data[2] * (m.data[5] * m.data[11] - m.data[7] * m.data[9]) +
                m.data[3] * (m.data[6] * m.data[9] - m.data[5] * m.data[10]));
        result.data[4] =
            d * (m.data[6] * (m.data[8] * m.data[15] - m.data[11] * m.data[12]) +
                m.data[7] * (m.data[10] * m.data[12] - m.data[8] * m.data[14]) +
                m.data[4] * (m.data[11] * m.data[14] - m.data[10] * m.data[15]));
        result.data[5] =
            d * (m.data[10] * (m.data[0] * m.data[15] - m.data[3] * m.data[12]) +
                m.data[11] * (m.data[2] * m.data[12] - m.data[0] * m.data[14]) +
                m.data[8] * (m.data[3] * m.data[14] - m.data[2] * m.data[15]));
        result.data[6] =
            d * (m.data[14] * (m.data[0] * m.data[7] - m.data[3] * m.data[4]) +
                m.data[15] * (m.data[2] * m.data[4] - m.data[0] * m.data[6]) +
                m.data[12] * (m.data[3] * m.data[6] - m.data[2] * m.data[7]));
        result.data[7] =
            d * (m.data[2] * (m.data[7] * m.data[8] - m.data[4] * m.data[11]) +
                m.data[3] * (m.data[4] * m.data[10] - m.data[6] * m.data[8]) +
                m.data[0] * (m.data[6] * m.data[11] - m.data[7] * m.data[10]));
        result.data[8] =
            d * (m.data[7] * (m.data[8] * m.data[13] - m.data[9] * m.data[12]) +
                m.data[4] * (m.data[9] * m.data[15] - m.data[11] * m.data[13]) +
                m.data[5] * (m.data[11] * m.data[12] - m.data[8] * m.data[15]));
        result.data[9] =
            d * (m.data[11] * (m.data[0] * m.data[13] - m.data[1] * m.data[12]) +
                m.data[8] * (m.data[1] * m.data[15] - m.data[3] * m.data[13]) +
                m.data[9] * (m.data[3] * m.data[12] - m.data[0] * m.data[15]));
        result.data[10] =
            d * (m.data[15] * (m.data[0] * m.data[5] - m.data[1] * m.data[4]) +
                m.data[12] * (m.data[1] * m.data[7] - m.data[3] * m.data[5]) +
                m.data[13] * (m.data[3] * m.data[4] - m.data[0] * m.data[7]));
        result.data[11] =
            d * (m.data[3] * (m.data[5] * m.data[8] - m.data[4] * m.data[9]) +
                m.data[0] * (m.data[7] * m.data[9] - m.data[5] * m.data[11]) +
                m.data[1] * (m.data[4] * m.data[11] - m.data[7] * m.data[8]));
        result.data[12] =
            d * (m.data[4] * (m.data[10] * m.data[13] - m.data[9] * m.data[14]) +
                m.data[5] * (m.data[8] * m.data[14] - m.data[10] * m.data[12]) +
                m.data[6] * (m.data[9] * m.data[12] - m.data[8] * m.data[13]));
        result.data[13] =
            d * (m.data[8] * (m.data[2] * m.data[13] - m.data[1] * m.data[14]) +
                m.data[9] * (m.data[0] * m.data[14] - m.data[2] * m.data[12]) +
                m.data[10] * (m.data[1] * m.data[12] - m.data[0] * m.data[13]));
        result.data[14] =
            d * (m.data[12] * (m.data[2] * m.data[5] - m.data[1] * m.data[6]) +
                m.data[13] * (m.data[0] * m.data[6] - m.data[2] * m.data[4]) +
                m.data[14] * (m.data[1] * m.data[4] - m.data[0] * m.data[5]));
        result.data[15] =
            d * (m.data[0] * (m.data[5] * m.data[10] - m.data[6] * m.data[9]) +
                m.data[1] * (m.data[6] * m.data[8] - m.data[4] * m.data[10]) +
                m.data[2] * (m.data[4] * m.data[9] - m.data[5] * m.data[8]));

        return result;
    }

    // IO stream operators
    std::ostream &operator<<(std::ostream &os, const Matrix44 &m) {
        os << "[ [ " << m.data[0] << " " << m.data[1] << " " << m.data[2] << " " << m.data[3] << " ]\n"
           << "  [ " << m.data[4] << " " << m.data[5] << " " << m.data[6] << " " << m.data[7] << " ]\n"
           << "  [ " << m.data[8] << " " << m.data[9] << " " << m.data[10] << " " << m.data[11] << " ]\n"
           << "  [ " << m.data[12] << " " << m.data[13] << " " << m.data[14] << " " << m.data[15] << " ] ]";
        return os;
    }
} // namespace TK
