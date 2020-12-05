#include "transform.hpp"

#include "aabb.hpp"

namespace TK {
    bool Transform::willSwapHandedness() const {
        // Coordinate system swaps handedness only when
        // determinant of upper left 3x3 matrix is negative
        tkFloat det = m.m_Entries[0] * (m.m_Entries[5] * m.m_Entries[10] - m.m_Entries[6] * m.m_Entries[9]) -
                      m.m_Entries[1] * (m.m_Entries[4] * m.m_Entries[10] - m.m_Entries[6] * m.m_Entries[8]) +
                      m.m_Entries[2] * (m.m_Entries[4] * m.m_Entries[9] - m.m_Entries[5] * m.m_Entries[8]);
        return det < 0;
    }

    bool Transform::isIdentity() const {
        return (
            m.m_Entries[0] == 1.0f && m.m_Entries[1] == 0.0f && m.m_Entries[2] == 0.0f && m.m_Entries[3] == 0.0f &&
            m.m_Entries[4] == 0.0f && m.m_Entries[5] == 1.0f && m.m_Entries[6] == 0.0f && m.m_Entries[7] == 0.0f &&
            m.m_Entries[8] == 0.0f && m.m_Entries[9] == 0.0f && m.m_Entries[10] == 1.0f && m.m_Entries[11] == 0.0f &&
            m.m_Entries[12] == 0.0f && m.m_Entries[13] == 0.0f && m.m_Entries[14] == 0.0f && m.m_Entries[15] == 1.0f);
    }

    const Matrix44& Transform::getMatrix() const {
        return m;
    }

    const Matrix44& Transform::getInverse() const {
        return mInv;
    }

    // IO stream operators
    std::istream& operator>>(std::istream& is, Transform& t) {
        Matrix44 m;
        is >> m;
        t = Transform(m);
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const Transform& t) {
        os << t.toString();
        return os;
    }

    // Transform operations
    Transform translate(const Vec3f& offset) {
        Matrix44 translationMatrix(1, 0, 0, offset.x, 0, 1, 0, offset.y, 0, 0, 1, offset.z, 0, 0, 0, 1);
        Matrix44 invMatrix(1, 0, 0, -offset.x, 0, 1, 0, -offset.y, 0, 0, 1, -offset.z, 0, 0, 0, 1);
        return Transform(translationMatrix, invMatrix);
    }

    Transform scale(tkFloat x, tkFloat y, tkFloat z) {
        Matrix44 scalingMatrix(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
        Matrix44 invMatrix(1 / x, 0, 0, 0, 0, 1 / y, 0, 0, 0, 0, 1 / z, 0, 0, 0, 0, 1);
        return Transform(scalingMatrix, invMatrix);
    }

    Transform rotate(const Vec3f& axis, tkFloat theta) {
        Vec3f a = normalize(axis);
        tkFloat sinTheta = std::sin(theta);
        tkFloat cosTheta = std::cos(theta);
        Matrix44 m;
        // Setting up basis vectors
        m.m_Entries[0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
        m.m_Entries[1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
        m.m_Entries[2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;

        m.m_Entries[4] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
        m.m_Entries[5] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
        m.m_Entries[6] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;

        m.m_Entries[8] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
        m.m_Entries[9] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
        m.m_Entries[10] = a.z * a.z + (1 - a.z * a.z) * cosTheta;

        return Transform(m, transpose(m));
    }

    Transform rotateX(tkFloat theta) {
        tkFloat sinTheta = std::sin(theta);
        tkFloat cosTheta = std::cos(theta);
        Matrix44 m(1, 0, 0, 0, 0, cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0, 0, 0, 0, 1);
        return Transform(m, transpose(m));
    }

    Transform rotateY(tkFloat theta) {
        tkFloat sinTheta = std::sin(theta);
        tkFloat cosTheta = std::cos(theta);
        Matrix44 m(cosTheta, 0, sinTheta, 0, 0, 1, 0, 0, -sinTheta, 0, cosTheta, 0, 0, 0, 0, 1);
        return Transform(m, transpose(m));
    }

    Transform rotateZ(tkFloat theta) {
        tkFloat sinTheta = std::sin(theta);
        tkFloat cosTheta = std::cos(theta);
        Matrix44 m(cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        return Transform(m, transpose(m));
    }

    // View Transform (Camera To World)
    Transform lookAt(const Point3f& eye, const Point3f& at, const Vec3f& up) {
        Matrix44 viewMatrix;
        Vec3f eyeVec = Vec3f(eye);
        Vec3f camZ = normalize(eye - at);
        Vec3f camX = normalize(cross(normalize(up), camZ));
        Vec3f camY = cross(camZ, camX);
        viewMatrix.m_Entries[0] = camX.x;
        viewMatrix.m_Entries[1] = camX.y;
        viewMatrix.m_Entries[2] = camX.z;
        viewMatrix.m_Entries[3] = -dot(camX, eyeVec);
        viewMatrix.m_Entries[4] = camY.x;
        viewMatrix.m_Entries[5] = camY.y;
        viewMatrix.m_Entries[6] = camY.z;
        viewMatrix.m_Entries[7] = -dot(camY, eyeVec);
        viewMatrix.m_Entries[8] = camZ.x;
        viewMatrix.m_Entries[9] = camZ.y;
        viewMatrix.m_Entries[10] = camZ.z;
        viewMatrix.m_Entries[11] = -dot(camZ, eyeVec);

        return Transform(inverse(viewMatrix), viewMatrix);
    }

    // Projection Transform
    Transform orthographic(tkFloat near, tkFloat far) {
        return scale(1, 1, 1 / (far - near)) * translate(Vec3f(0, 0, near));
    }

    Transform perspective(tkFloat fovy, tkFloat aspect, tkFloat near, tkFloat far) {
        tkFloat fMinusN = far - near;
        tkFloat oneOverTan = -1 / std::tan(degToRad(fovy) * 0.5);
        Matrix44 persp(oneOverTan / aspect, 0, 0, 0, 0, oneOverTan, 0, 0, 0, 0, far / fMinusN,
                       near * far / fMinusN, 0, 0, 1, 0);
        return Transform(persp);
    }
}  // namespace TK