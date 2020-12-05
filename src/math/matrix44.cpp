#include "matrix44.hpp"

namespace TK {
    Matrix44::Matrix44() {
        m_Entries[0]  = 1.0f; m_Entries[1]  = 0.0f; m_Entries[2]  = 0.0f; m_Entries[3]  = 0.0f;
        m_Entries[4]  = 0.0f; m_Entries[5]  = 1.0f; m_Entries[6]  = 0.0f; m_Entries[7]  = 0.0f;
        m_Entries[8]  = 0.0f; m_Entries[9]  = 0.0f; m_Entries[10] = 1.0f; m_Entries[11] = 0.0f;
        m_Entries[12] = 0.0f; m_Entries[13] = 0.0f; m_Entries[14] = 0.0f; m_Entries[15] = 1.0f;
    }

    Matrix44::Matrix44(tkFloat d[16]) {
        memcpy(m_Entries, d, 16 * sizeof(tkFloat));
    }

    Matrix44::Matrix44(tkFloat f00, tkFloat f01, tkFloat f02, tkFloat f03,
                    tkFloat f10, tkFloat f11, tkFloat f12, tkFloat f13,
                    tkFloat f20, tkFloat f21, tkFloat f22, tkFloat f23,
                    tkFloat f30, tkFloat f31, tkFloat f32, tkFloat f33) {
        m_Entries[0] = f00;
        m_Entries[1] = f01;
        m_Entries[2] = f02;
        m_Entries[3] = f03;
        m_Entries[4] = f10;
        m_Entries[5] = f11;
        m_Entries[6] = f12;
        m_Entries[7] = f13;
        m_Entries[8] = f20;
        m_Entries[9] = f21;
        m_Entries[10] = f22;
        m_Entries[11] = f23;
        m_Entries[12] = f30;
        m_Entries[13] = f31;
        m_Entries[14] = f32;
        m_Entries[15] = f33;
    }

    Matrix44& Matrix44::operator*=(const Matrix44& m) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m_Entries[i * 4 + j] = m_Entries[i * 4] * m.m_Entries[j] + m_Entries[i * 4 + 1] * m.m_Entries[4 + j] +
                                     m_Entries[i * 4 + 2] * m.m_Entries[8 + j] +
                                     m_Entries[i * 4 + 3] * m.m_Entries[12 + j];
            }
        }
        return *this;
    }

    bool Matrix44::operator==(const Matrix44& m) const {
        for (int i = 0; i < 16; ++i)
            if (m_Entries[i] != m.m_Entries[i])
                return false;
        return true;
    }

    bool Matrix44::operator!=(const Matrix44& m) const {
        for (int i = 0; i < 16; ++i)
            if (m_Entries[i] != m.m_Entries[i])
                return true;
        return false;
    }

    // Binary operators
    Matrix44 operator*(const Matrix44& m1, const Matrix44& m2) {
        Matrix44 m;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m.m_Entries[i * 4 + j] = m1.m_Entries[i * 4] * m2.m_Entries[j] +
                                         m1.m_Entries[i * 4 + 1] * m2.m_Entries[4 + j] +
                                         m1.m_Entries[i * 4 + 2] * m2.m_Entries[8 + j] +
                                         m1.m_Entries[i * 4 + 3] * m2.m_Entries[12 + j];
            }
        }
        return m;
    }

    // Matrix operations
    tkFloat Matrix44::determinant() const {
        return (m_Entries[0] * m_Entries[5] - m_Entries[1] * m_Entries[4]) *
                   (m_Entries[10] * m_Entries[15] - m_Entries[11] * m_Entries[14]) -
               (m_Entries[0] * m_Entries[6] - m_Entries[2] * m_Entries[4]) *
                   (m_Entries[9] * m_Entries[15] - m_Entries[11] * m_Entries[13]) +
               (m_Entries[0] * m_Entries[7] - m_Entries[3] * m_Entries[4]) *
                   (m_Entries[9] * m_Entries[14] - m_Entries[10] * m_Entries[13]) +
               (m_Entries[1] * m_Entries[6] - m_Entries[2] * m_Entries[5]) *
                   (m_Entries[8] * m_Entries[15] - m_Entries[11] * m_Entries[12]) -
               (m_Entries[1] * m_Entries[7] - m_Entries[3] * m_Entries[5]) *
                   (m_Entries[8] * m_Entries[14] - m_Entries[10] * m_Entries[12]) +
               (m_Entries[2] * m_Entries[7] - m_Entries[3] * m_Entries[6]) *
                   (m_Entries[8] * m_Entries[13] - m_Entries[9] * m_Entries[12]);
    }

    std::string Matrix44::toString() const {
        std::ostringstream oss;
        oss << "Matrix44 [\n";
        for (int i = 0; i < 4; ++i) {
            oss << "    ";
            for (int j = 0; j < 4; ++j) {
                oss << m_Entries[i * 4 + j];
                if (j != 3)
                    oss << ", ";
            }
            oss << "\n";
        }
        oss << "]";
        return oss.str();
    }

    Matrix44 transpose(const Matrix44& m) {
        return Matrix44(m.m_Entries[0], m.m_Entries[4], m.m_Entries[8], m.m_Entries[12], m.m_Entries[1], m.m_Entries[5],
                        m.m_Entries[9], m.m_Entries[13], m.m_Entries[2], m.m_Entries[6], m.m_Entries[10], m.m_Entries[14],
                        m.m_Entries[3], m.m_Entries[7], m.m_Entries[11], m.m_Entries[15]);
    }

    Matrix44 inverse(const Matrix44& m) {
        tkFloat d = m.determinant();
        if (std::abs(d) < TK_EPSILON)
            return Matrix44();

        // use Cramer's rule
        d = 1.0 / d;
        Matrix44 result;
        result.m_Entries[0] =
            d * (m.m_Entries[5] * (m.m_Entries[10] * m.m_Entries[15] - m.m_Entries[11] * m.m_Entries[14]) +
                 m.m_Entries[6] * (m.m_Entries[11] * m.m_Entries[13] - m.m_Entries[9] * m.m_Entries[15]) +
                 m.m_Entries[7] * (m.m_Entries[9] * m.m_Entries[14] - m.m_Entries[10] * m.m_Entries[13]));
        result.m_Entries[1] =
            d * (m.m_Entries[9] * (m.m_Entries[2] * m.m_Entries[15] - m.m_Entries[3] * m.m_Entries[14]) +
                 m.m_Entries[10] * (m.m_Entries[3] * m.m_Entries[13] - m.m_Entries[1] * m.m_Entries[15]) +
                 m.m_Entries[11] * (m.m_Entries[1] * m.m_Entries[14] - m.m_Entries[2] * m.m_Entries[13]));
        result.m_Entries[2] = d * (m.m_Entries[13] * (m.m_Entries[2] * m.m_Entries[7] - m.m_Entries[3] * m.m_Entries[6]) +
                                 m.m_Entries[14] * (m.m_Entries[3] * m.m_Entries[5] - m.m_Entries[1] * m.m_Entries[7]) +
                                 m.m_Entries[15] * (m.m_Entries[1] * m.m_Entries[6] - m.m_Entries[2] * m.m_Entries[5]));
        result.m_Entries[3] =
            d * (m.m_Entries[1] * (m.m_Entries[7] * m.m_Entries[10] - m.m_Entries[6] * m.m_Entries[11]) +
                 m.m_Entries[2] * (m.m_Entries[5] * m.m_Entries[11] - m.m_Entries[7] * m.m_Entries[9]) +
                 m.m_Entries[3] * (m.m_Entries[6] * m.m_Entries[9] - m.m_Entries[5] * m.m_Entries[10]));
        result.m_Entries[4] =
            d * (m.m_Entries[6] * (m.m_Entries[8] * m.m_Entries[15] - m.m_Entries[11] * m.m_Entries[12]) +
                 m.m_Entries[7] * (m.m_Entries[10] * m.m_Entries[12] - m.m_Entries[8] * m.m_Entries[14]) +
                 m.m_Entries[4] * (m.m_Entries[11] * m.m_Entries[14] - m.m_Entries[10] * m.m_Entries[15]));
        result.m_Entries[5] =
            d * (m.m_Entries[10] * (m.m_Entries[0] * m.m_Entries[15] - m.m_Entries[3] * m.m_Entries[12]) +
                 m.m_Entries[11] * (m.m_Entries[2] * m.m_Entries[12] - m.m_Entries[0] * m.m_Entries[14]) +
                 m.m_Entries[8] * (m.m_Entries[3] * m.m_Entries[14] - m.m_Entries[2] * m.m_Entries[15]));
        result.m_Entries[6] = d * (m.m_Entries[14] * (m.m_Entries[0] * m.m_Entries[7] - m.m_Entries[3] * m.m_Entries[4]) +
                                 m.m_Entries[15] * (m.m_Entries[2] * m.m_Entries[4] - m.m_Entries[0] * m.m_Entries[6]) +
                                 m.m_Entries[12] * (m.m_Entries[3] * m.m_Entries[6] - m.m_Entries[2] * m.m_Entries[7]));
        result.m_Entries[7] =
            d * (m.m_Entries[2] * (m.m_Entries[7] * m.m_Entries[8] - m.m_Entries[4] * m.m_Entries[11]) +
                 m.m_Entries[3] * (m.m_Entries[4] * m.m_Entries[10] - m.m_Entries[6] * m.m_Entries[8]) +
                 m.m_Entries[0] * (m.m_Entries[6] * m.m_Entries[11] - m.m_Entries[7] * m.m_Entries[10]));
        result.m_Entries[8] =
            d * (m.m_Entries[7] * (m.m_Entries[8] * m.m_Entries[13] - m.m_Entries[9] * m.m_Entries[12]) +
                 m.m_Entries[4] * (m.m_Entries[9] * m.m_Entries[15] - m.m_Entries[11] * m.m_Entries[13]) +
                 m.m_Entries[5] * (m.m_Entries[11] * m.m_Entries[12] - m.m_Entries[8] * m.m_Entries[15]));
        result.m_Entries[9] =
            d * (m.m_Entries[11] * (m.m_Entries[0] * m.m_Entries[13] - m.m_Entries[1] * m.m_Entries[12]) +
                 m.m_Entries[8] * (m.m_Entries[1] * m.m_Entries[15] - m.m_Entries[3] * m.m_Entries[13]) +
                 m.m_Entries[9] * (m.m_Entries[3] * m.m_Entries[12] - m.m_Entries[0] * m.m_Entries[15]));
        result.m_Entries[10] =
            d * (m.m_Entries[15] * (m.m_Entries[0] * m.m_Entries[5] - m.m_Entries[1] * m.m_Entries[4]) +
                 m.m_Entries[12] * (m.m_Entries[1] * m.m_Entries[7] - m.m_Entries[3] * m.m_Entries[5]) +
                 m.m_Entries[13] * (m.m_Entries[3] * m.m_Entries[4] - m.m_Entries[0] * m.m_Entries[7]));
        result.m_Entries[11] =
            d * (m.m_Entries[3] * (m.m_Entries[5] * m.m_Entries[8] - m.m_Entries[4] * m.m_Entries[9]) +
                 m.m_Entries[0] * (m.m_Entries[7] * m.m_Entries[9] - m.m_Entries[5] * m.m_Entries[11]) +
                 m.m_Entries[1] * (m.m_Entries[4] * m.m_Entries[11] - m.m_Entries[7] * m.m_Entries[8]));
        result.m_Entries[12] =
            d * (m.m_Entries[4] * (m.m_Entries[10] * m.m_Entries[13] - m.m_Entries[9] * m.m_Entries[14]) +
                 m.m_Entries[5] * (m.m_Entries[8] * m.m_Entries[14] - m.m_Entries[10] * m.m_Entries[12]) +
                 m.m_Entries[6] * (m.m_Entries[9] * m.m_Entries[12] - m.m_Entries[8] * m.m_Entries[13]));
        result.m_Entries[13] =
            d * (m.m_Entries[8] * (m.m_Entries[2] * m.m_Entries[13] - m.m_Entries[1] * m.m_Entries[14]) +
                 m.m_Entries[9] * (m.m_Entries[0] * m.m_Entries[14] - m.m_Entries[2] * m.m_Entries[12]) +
                 m.m_Entries[10] * (m.m_Entries[1] * m.m_Entries[12] - m.m_Entries[0] * m.m_Entries[13]));
        result.m_Entries[14] =
            d * (m.m_Entries[12] * (m.m_Entries[2] * m.m_Entries[5] - m.m_Entries[1] * m.m_Entries[6]) +
                 m.m_Entries[13] * (m.m_Entries[0] * m.m_Entries[6] - m.m_Entries[2] * m.m_Entries[4]) +
                 m.m_Entries[14] * (m.m_Entries[1] * m.m_Entries[4] - m.m_Entries[0] * m.m_Entries[5]));
        result.m_Entries[15] =
            d * (m.m_Entries[0] * (m.m_Entries[5] * m.m_Entries[10] - m.m_Entries[6] * m.m_Entries[9]) +
                 m.m_Entries[1] * (m.m_Entries[6] * m.m_Entries[8] - m.m_Entries[4] * m.m_Entries[10]) +
                 m.m_Entries[2] * (m.m_Entries[4] * m.m_Entries[9] - m.m_Entries[5] * m.m_Entries[8]));

        return result;
    }

    // IO stream operators
    std::istream& operator>>(std::istream& is, Matrix44& m) {
        is >> m.m_Entries[0] >> m.m_Entries[1] >> m.m_Entries[2] >> m.m_Entries[3]
           >> m.m_Entries[4] >> m.m_Entries[5] >> m.m_Entries[6] >> m.m_Entries[7]
           >> m.m_Entries[8] >> m.m_Entries[9] >> m.m_Entries[10] >> m.m_Entries[11]
           >> m.m_Entries[12] >> m.m_Entries[13] >> m.m_Entries[14] >> m.m_Entries[15];
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const Matrix44& m) {
        os << m.toString() << "\n";
        return os;
    }
}  // namespace TK
