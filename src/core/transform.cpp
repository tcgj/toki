#include "transform.hpp"

#include "aabb.hpp"

namespace TK {
    bool Transform::willSwapHandedness() const {
        // Coordinate system swaps handedness only when
        // determinant of upper left 3x3 matrix is negative
        tkFloat det = m.entries[0] * (m.entries[5] * m.entries[10] - m.entries[6] * m.entries[9]) -
                      m.entries[1] * (m.entries[4] * m.entries[10] - m.entries[6] * m.entries[8]) +
                      m.entries[2] * (m.entries[4] * m.entries[9] - m.entries[5] * m.entries[8]);
        return det < 0;
    }
    bool Transform::isIdentity() const {
        return (
            m.entries[0] == 1.0f && m.entries[1] == 0.0f && m.entries[2] == 0.0f && m.entries[3] == 0.0f &&
            m.entries[4] == 0.0f && m.entries[5] == 1.0f && m.entries[6] == 0.0f && m.entries[7] == 0.0f &&
            m.entries[8] == 0.0f && m.entries[9] == 0.0f && m.entries[10] == 1.0f && m.entries[11] == 0.0f &&
            m.entries[12] == 0.0f && m.entries[13] == 0.0f && m.entries[14] == 0.0f && m.entries[15] == 1.0f);
    }
    const Matrix44& Transform::getMatrix() const {
        return m;
    }
    const Matrix44& Transform::getInverse() const {
        return mInv;
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
        m.entries[0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
        m.entries[1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
        m.entries[2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;

        m.entries[4] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
        m.entries[5] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
        m.entries[6] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;

        m.entries[8] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
        m.entries[9] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
        m.entries[10] = a.z * a.z + (1 - a.z * a.z) * cosTheta;

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
        viewMatrix.entries[0] = camX.x;
        viewMatrix.entries[1] = camX.y;
        viewMatrix.entries[2] = camX.z;
        viewMatrix.entries[3] = -dot(camX, eyeVec);
        viewMatrix.entries[4] = camY.x;
        viewMatrix.entries[5] = camY.y;
        viewMatrix.entries[6] = camY.z;
        viewMatrix.entries[7] = -dot(camY, eyeVec);
        viewMatrix.entries[8] = camZ.x;
        viewMatrix.entries[9] = camZ.y;
        viewMatrix.entries[10] = camZ.z;
        viewMatrix.entries[11] = -dot(camZ, eyeVec);

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