#include "matrix44.hpp"

#include "math/math.hpp"

namespace TK {
    Matrix44::Matrix44() {
        entries[0]  = 1.0f; entries[1]  = 0.0f; entries[2]  = 0.0f; entries[3]  = 0.0f;
        entries[4]  = 0.0f; entries[5]  = 1.0f; entries[6]  = 0.0f; entries[7]  = 0.0f;
        entries[8]  = 0.0f; entries[9]  = 0.0f; entries[10] = 1.0f; entries[11] = 0.0f;
        entries[12] = 0.0f; entries[13] = 0.0f; entries[14] = 0.0f; entries[15] = 1.0f;
    }
    Matrix44::Matrix44(tkFloat d[16]) {
        memcpy(entries, d, 16 * sizeof(tkFloat));
    }
    Matrix44::Matrix44(tkFloat f00, tkFloat f01, tkFloat f02, tkFloat f03,
                    tkFloat f10, tkFloat f11, tkFloat f12, tkFloat f13,
                    tkFloat f20, tkFloat f21, tkFloat f22, tkFloat f23,
                    tkFloat f30, tkFloat f31, tkFloat f32, tkFloat f33) {
        entries[0] = f00;
        entries[1] = f01;
        entries[2] = f02;
        entries[3] = f03;
        entries[4] = f10;
        entries[5] = f11;
        entries[6] = f12;
        entries[7] = f13;
        entries[8] = f20;
        entries[9] = f21;
        entries[10] = f22;
        entries[11] = f23;
        entries[12] = f30;
        entries[13] = f31;
        entries[14] = f32;
        entries[15] = f33;
    }

    Matrix44 &Matrix44::operator*=(const Matrix44 &m) {
        for (tkUInt i = 0; i < 4; ++i) {
            for (tkUInt j = 0; j < 4; ++j) {
                entries[i * 4 + j] = entries[i * 4] * m.entries[j] +
                                entries[i * 4 + 1] * m.entries[4 + j] +
                                entries[i * 4 + 2] * m.entries[8 + j] +
                                entries[i * 4 + 3] * m.entries[12 + j];
            }
        }
        return *this;
    }
    bool Matrix44::operator==(const Matrix44 &m) const {
        for (tkUInt i = 0; i < 16; ++i)
            if (entries[i] != m.entries[i]) return false;
        return true;
    }
    bool Matrix44::operator!=(const Matrix44 &m) const {
        for (tkUInt i = 0; i < 16; ++i)
            if (entries[i] != m.entries[i]) return true;
        return false;
    }

    // Binary operators
    Matrix44 operator*(const Matrix44 &m1, const Matrix44 &m2) {
        Matrix44 m;
        for (tkUInt i = 0; i < 4; ++i) {
            for (tkUInt j = 0; j < 4; ++j) {
                m.entries[i * 4 + j] =
                    m1.entries[i * 4] * m2.entries[j] + m1.entries[i * 4 + 1] * m2.entries[4 + j] +
                    m1.entries[i * 4 + 2] * m2.entries[8 + j] + m1.entries[i * 4 + 3] * m2.entries[12 + j];
            }
        }
        return m;
    }

    // Matrix operations
    tkFloat Matrix44::determinant() const {
        return (entries[0] * entries[5] - entries[1] * entries[4]) *
                (entries[10] * entries[15] - entries[11] * entries[14]) -
            (entries[0] * entries[6] - entries[2] * entries[4]) *
                (entries[9] * entries[15] - entries[11] * entries[13]) +
            (entries[0] * entries[7] - entries[3] * entries[4]) *
                (entries[9] * entries[14] - entries[10] * entries[13]) +
            (entries[1] * entries[6] - entries[2] * entries[5]) *
                (entries[8] * entries[15] - entries[11] * entries[12]) -
            (entries[1] * entries[7] - entries[3] * entries[5]) *
                (entries[8] * entries[14] - entries[10] * entries[12]) +
            (entries[2] * entries[7] - entries[3] * entries[6]) *
                (entries[8] * entries[13] - entries[9] * entries[12]);
    }

    Matrix44 transpose(const Matrix44 &m) {
        return Matrix44(m.entries[0], m.entries[4], m.entries[8], m.entries[12],
                        m.entries[1], m.entries[5], m.entries[9], m.entries[13],
                        m.entries[2], m.entries[6], m.entries[10], m.entries[14],
                        m.entries[3], m.entries[7], m.entries[11], m.entries[15]);
    }
    Matrix44 inverse(const Matrix44 &m) {
        tkFloat d = m.determinant();
        if (std::abs(d) < TK_EPSILON)
            return Matrix44();

        // use Cramer's rule
        d = 1.0 / d;
        Matrix44 result;
        result.entries[0] =
            d * (m.entries[5] * (m.entries[10] * m.entries[15] - m.entries[11] * m.entries[14]) +
                m.entries[6] * (m.entries[11] * m.entries[13] - m.entries[9] * m.entries[15]) +
                m.entries[7] * (m.entries[9] * m.entries[14] - m.entries[10] * m.entries[13]));
        result.entries[1] =
            d * (m.entries[9] * (m.entries[2] * m.entries[15] - m.entries[3] * m.entries[14]) +
                m.entries[10] * (m.entries[3] * m.entries[13] - m.entries[1] * m.entries[15]) +
                m.entries[11] * (m.entries[1] * m.entries[14] - m.entries[2] * m.entries[13]));
        result.entries[2] =
            d * (m.entries[13] * (m.entries[2] * m.entries[7] - m.entries[3] * m.entries[6]) +
                m.entries[14] * (m.entries[3] * m.entries[5] - m.entries[1] * m.entries[7]) +
                m.entries[15] * (m.entries[1] * m.entries[6] - m.entries[2] * m.entries[5]));
        result.entries[3] =
            d * (m.entries[1] * (m.entries[7] * m.entries[10] - m.entries[6] * m.entries[11]) +
                m.entries[2] * (m.entries[5] * m.entries[11] - m.entries[7] * m.entries[9]) +
                m.entries[3] * (m.entries[6] * m.entries[9] - m.entries[5] * m.entries[10]));
        result.entries[4] =
            d * (m.entries[6] * (m.entries[8] * m.entries[15] - m.entries[11] * m.entries[12]) +
                m.entries[7] * (m.entries[10] * m.entries[12] - m.entries[8] * m.entries[14]) +
                m.entries[4] * (m.entries[11] * m.entries[14] - m.entries[10] * m.entries[15]));
        result.entries[5] =
            d * (m.entries[10] * (m.entries[0] * m.entries[15] - m.entries[3] * m.entries[12]) +
                m.entries[11] * (m.entries[2] * m.entries[12] - m.entries[0] * m.entries[14]) +
                m.entries[8] * (m.entries[3] * m.entries[14] - m.entries[2] * m.entries[15]));
        result.entries[6] =
            d * (m.entries[14] * (m.entries[0] * m.entries[7] - m.entries[3] * m.entries[4]) +
                m.entries[15] * (m.entries[2] * m.entries[4] - m.entries[0] * m.entries[6]) +
                m.entries[12] * (m.entries[3] * m.entries[6] - m.entries[2] * m.entries[7]));
        result.entries[7] =
            d * (m.entries[2] * (m.entries[7] * m.entries[8] - m.entries[4] * m.entries[11]) +
                m.entries[3] * (m.entries[4] * m.entries[10] - m.entries[6] * m.entries[8]) +
                m.entries[0] * (m.entries[6] * m.entries[11] - m.entries[7] * m.entries[10]));
        result.entries[8] =
            d * (m.entries[7] * (m.entries[8] * m.entries[13] - m.entries[9] * m.entries[12]) +
                m.entries[4] * (m.entries[9] * m.entries[15] - m.entries[11] * m.entries[13]) +
                m.entries[5] * (m.entries[11] * m.entries[12] - m.entries[8] * m.entries[15]));
        result.entries[9] =
            d * (m.entries[11] * (m.entries[0] * m.entries[13] - m.entries[1] * m.entries[12]) +
                m.entries[8] * (m.entries[1] * m.entries[15] - m.entries[3] * m.entries[13]) +
                m.entries[9] * (m.entries[3] * m.entries[12] - m.entries[0] * m.entries[15]));
        result.entries[10] =
            d * (m.entries[15] * (m.entries[0] * m.entries[5] - m.entries[1] * m.entries[4]) +
                m.entries[12] * (m.entries[1] * m.entries[7] - m.entries[3] * m.entries[5]) +
                m.entries[13] * (m.entries[3] * m.entries[4] - m.entries[0] * m.entries[7]));
        result.entries[11] =
            d * (m.entries[3] * (m.entries[5] * m.entries[8] - m.entries[4] * m.entries[9]) +
                m.entries[0] * (m.entries[7] * m.entries[9] - m.entries[5] * m.entries[11]) +
                m.entries[1] * (m.entries[4] * m.entries[11] - m.entries[7] * m.entries[8]));
        result.entries[12] =
            d * (m.entries[4] * (m.entries[10] * m.entries[13] - m.entries[9] * m.entries[14]) +
                m.entries[5] * (m.entries[8] * m.entries[14] - m.entries[10] * m.entries[12]) +
                m.entries[6] * (m.entries[9] * m.entries[12] - m.entries[8] * m.entries[13]));
        result.entries[13] =
            d * (m.entries[8] * (m.entries[2] * m.entries[13] - m.entries[1] * m.entries[14]) +
                m.entries[9] * (m.entries[0] * m.entries[14] - m.entries[2] * m.entries[12]) +
                m.entries[10] * (m.entries[1] * m.entries[12] - m.entries[0] * m.entries[13]));
        result.entries[14] =
            d * (m.entries[12] * (m.entries[2] * m.entries[5] - m.entries[1] * m.entries[6]) +
                m.entries[13] * (m.entries[0] * m.entries[6] - m.entries[2] * m.entries[4]) +
                m.entries[14] * (m.entries[1] * m.entries[4] - m.entries[0] * m.entries[5]));
        result.entries[15] =
            d * (m.entries[0] * (m.entries[5] * m.entries[10] - m.entries[6] * m.entries[9]) +
                m.entries[1] * (m.entries[6] * m.entries[8] - m.entries[4] * m.entries[10]) +
                m.entries[2] * (m.entries[4] * m.entries[9] - m.entries[5] * m.entries[8]));

        return result;
    }

    // IO stream operators
    std::ostream &operator<<(std::ostream &os, const Matrix44 &m) {
        os << "[ [ " << m.entries[0] << " " << m.entries[1] << " " << m.entries[2] << " " << m.entries[3] << " ]\n"
           << "  [ " << m.entries[4] << " " << m.entries[5] << " " << m.entries[6] << " " << m.entries[7] << " ]\n"
           << "  [ " << m.entries[8] << " " << m.entries[9] << " " << m.entries[10] << " " << m.entries[11] << " ]\n"
           << "  [ " << m.entries[12] << " " << m.entries[13] << " " << m.entries[14] << " " << m.entries[15] << " ] ]";
        return os;
    }
} // namespace TK
