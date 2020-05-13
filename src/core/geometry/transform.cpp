#include "transform.hpp"

namespace TK {
    template <typename T>
    Point3<T> Transform::operator()(const Point3<T> &p) const {
        T px = p.x, py = p.y, pz = p.z;
        T x = m.data[0] * px + m.data[1] * py + m.data[2] * pz + m.data[3];
        T y = m.data[4] * px + m.data[5] * py + m.data[6] * pz + m.data[7];
        T z = m.data[8] * px + m.data[9] * py + m.data[10] * pz + m.data[11];
        T w = m.data[12] * px + m.data[13] * py + m.data[14] * pz + m.data[15];
        if (wp == 1)
            return Point3<T>(x, y, z);
        else
            return Point3<T>(x, y, z) / w;
    }
    template <typename T>
    Vec3<T> Transform::operator()(const Vec3<T> &v, bool isNormal = false) const {
        T vx = v.x, vy = v.y, vz = v.z;

        if (isNormal) {
            return Vec3<T>(
                mInv.data[0] * px + mInv.data[4] * py + mInv.data[8] * pz,
                mInv.data[1] * px + mInv.data[5] * py + mInv.data[9] * pz,
                mInv.data[2] * px + mInv.data[6] * py + mInv.data[10] * pz);
        } else {
            return Vec3<T>(
                m.data[0] * px + m.data[1] * py + m.data[2] * pz,
                m.data[4] * px + m.data[5] * py + m.data[6] * pz,
                m.data[8] * px + m.data[9] * py + m.data[10] * pz);
        }
    }
    template <typename T>
    AABB<T> Transform::operator()(const AABB<T> &bb) const {
        const Transform &mat = *this;
        AABB<T> out(mat(bb.corner(0)));
        for (tkInt i = 1; i < 8; ++i) {
            out = bbUnion(out, mat(bb.corner(i)));
        }
        return out;
    }
    Ray Transform::operator()(const Ray &r) const {
        tkPoint3f o = (*this)(r.o);
        tkVec3f d = (*this)(r.d);

        return Ray(o, d, r.tMax/*, r.time, r.medium*/);
    }

    bool Transform::willSwapHandedness() const {
        // Coordinate system swaps handedness only when
        // determinant of upper left 3x3 matrix is negative
        tkFloat det =
            m.data[0] * (m.data[5] * m.data[10] - m.data[6] * m.data[9]) -
            m.data[1] * (m.data[4] * m.data[10] - m.data[6] * m.data[8]) +
            m.data[2] * (m.data[4] * m.data[9] - m.data[5] * m.data[8]);
        return det < 0;
    }
    bool Transform::isIdentity() const {
        return (m.data[0] == 1.0f && m.data[1] == 0.0f && m.data[2] == 0.0f && m.data[3] == 0.0f &&
                m.data[4] == 0.0f && m.data[5] == 1.0f && m.data[6] == 0.0f && m.data[7] == 0.0f &&
                m.data[8] == 0.0f && m.data[9] == 0.0f && m.data[10] == 1.0f && m.data[11] == 0.0f &&
                m.data[12] == 0.0f && m.data[13] == 0.0f && m.data[14] == 0.0f && m.data[15] == 1.0f);
    }
    const Matrix44 &Transform::getMatrix() const {
        return m;
    }
    const Matrix44 &Transform::getInverse() const {
        return mInv;
    }

    // Transform operations
    Transform translate(const tkVec3f &offset) {
        Matrix44 translationMatrix(1, 0, 0, offset.x,
                                   0, 1, 0, offset.y,
                                   0, 0, 1, offset.z,
                                   0, 0, 0, 1);
        Matrix44 invMatrix(1, 0, 0, -offset.x,
                           0, 1, 0, -offset.y,
                           0, 0, 1, -offset.z,
                           0, 0, 0, 1);
        return Transform(translationMatrix, invMatrix);
    }
    Transform scale(tkFloat x, tkFloat y, tkFloat z) {
        Matrix44 scalingMatrix(x, 0, 0, 0,
                               0, y, 0, 0,
                               0, 0, z, 0,
                               0, 0, 0, 1);
        Matrix44 invMatrix(1/x, 0, 0, 0,
                           0, 1/y, 0, 0,
                           0, 0, 1/z, 0,
                           0, 0, 0, 1);
        return Transform(scalingMatrix, invMatrix);
    }
    Transform rotate(const tkVec3f &axis, tkFloat theta) {
        tkVec3f a = normalize(axis);
        tkFloat sinTheta = std::sin(theta);
        tkFloat cosTheta = std::cos(theta);
        Matrix44 m;
        // Setting up basis vectors
        m.data[0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
        m.data[1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
        m.data[2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;

        m.data[4] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
        m.data[5] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
        m.data[6] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;

        m.data[8] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
        m.data[9] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
        m.data[10] = a.z * a.z + (1 - a.z * a.z) * cosTheta;

        return Transform(m, transpose(m));
    }
    Transform rotateX(tkFloat theta) {
        tkFloat sinTheta = std::sin(theta);
        tkFloat cosTheta = std::cos(theta);
        Matrix44 m(1, 0, 0, 0,
                   0, cosTheta, -sinTheta, 0,
                   0, sinTheta, cosTheta, 0,
                   0, 0, 0, 1);
        return Transform(m, transpose(m));
    }
    Transform rotateY(tkFloat theta) {
        tkFloat sinTheta = std::sin(theta);
        tkFloat cosTheta = std::cos(theta);
        Matrix44 m(cosTheta, 0, sinTheta, 0,
                   0, 1, 0, 0,
                   -sinTheta, 0, cosTheta, 0,
                   0, 0, 0, 1);
        return Transform(m, transpose(m));
    }
    Transform rotateZ(tkFloat theta) {
        tkFloat sinTheta = std::sin(theta);
        tkFloat cosTheta = std::cos(theta);
        Matrix44 m(cosTheta, -sinTheta, 0, 0,
                   sinTheta, cosTheta, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1);
        return Transform(m, transpose(m));
    }
    // View Transform
    Transform lookAt(const tkPoint3f &eye, const tkPoint3f &at, const tkVec3f &up) {
        Matrix44 viewMatrix;
        tkVec3f eyeVec = tkVec3f(eye);
        tkVec3f camZ = normalize(eye - at);
        tkVec3f camX = normalize(cross(normalize(up), camZ));
        tkVec3f camY = cross(camZ, camX);
        viewMatrix.data[0] = camX.x;
        viewMatrix.data[1] = camX.y;
        viewMatrix.data[2] = camX.z;
        viewMatrix.data[3] = -dot(camX, eyeVec);
        viewMatrix.data[4] = camY.x;
        viewMatrix.data[5] = camY.y;
        viewMatrix.data[6] = camY.z;
        viewMatrix.data[7] = -dot(camY, eyeVec);
        viewMatrix.data[8] = camZ.x;
        viewMatrix.data[9] = camZ.y;
        viewMatrix.data[10] = camZ.z;
        viewMatrix.data[11] = -dot(camZ, eyeVec);

        return Transform(viewMatrix, inverse(viewMatrix));
    }
} // namespace TK