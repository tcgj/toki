#include "quaternion.hpp"

#include "core/transform.hpp"

// All implementations here follow the right-hand coordinate system
namespace TK {
    Quaternion::Quaternion(const Transform &t) {
        // constructor derived from pbrt
        // Working backwards given quaternion to rotation matrix formula
        const Matrix44 &m = t.getMatrix();
        tkFloat trace = m.data[0] + m.data[5] + m.data[10];
        if (trace > 0.0f) {
            // compute w from trace, then xyz
            // 4w^2 = [0,0] + [1,1] + [2,2] + [3,3] (but [3,3] == 1)
            tkFloat s = std::sqrt(trace + 1.0f);
            w = s * 0.5f;
            s = 0.5f / s;
            xyz.x = (m.data[6] - m.data[9]) * s;
            xyz.y = (m.data[8] - m.data[2]) * s;
            xyz.z = (m.data[1] - m.data[4]) * s;
        } else {
            const tkUInt next[3] = {1, 2, 0};
            tkFloat q[3];
            tkUInt i = 0;
            if (m.data[5] > m.data[0])
                i = 1;
            if (m.data[10] > m.data[5])
                i = 2;
            tkUInt j = next[i];
            tkUInt k = next[j];
            tkFloat s = std::sqrt(1.0f + m.data[i * 4 + i] - m.data[j * 4 + j] - m.data[k * 4 + k]);
            q[i] = s * 0.5f;
            if (s != 0.0f)
                s = 0.5f / s; // Should never be zero
            q[j] = (m.data[i * 4 + j] + m.data[j * 4 + i]) * s;
            q[k] = (m.data[i * 4 + k] + m.data[k * 4 + i]) * s;
            w = (m.data[j * 4 + k] - m.data[k * 4 + j]) * s;
            xyz.x = q[0];
            xyz.y = q[1];
            xyz.z = q[2];
        }
    }

    // Assumes that quaternion is normalized
    Transform Quaternion::toTransform() const {
        tkFloat xx = xyz.x * xyz.x;
        tkFloat yy = xyz.y * xyz.y;
        tkFloat zz = xyz.z * xyz.z;

        tkFloat xy = xyz.x * xyz.y;
        tkFloat xz = xyz.x * xyz.z;
        tkFloat yz = xyz.y * xyz.z;

        tkFloat wx = w * xyz.x;
        tkFloat wy = w * xyz.y;
        tkFloat wz = w * xyz.z;

        Matrix44 m(1 - 2 * (yy + zz), 2 * (xy + wz), 2 * (xz - wy), 0,
                   2 * (xy - wz), 1 - 2 * (xx + zz), 2 * (yz + wx), 0,
                   2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (xx + yy), 0,
                   0, 0, 0, 1);
        return Transform(m, transpose(m));
    }

    // Assumes that both q1 and q2 are unit quaternions
    Quaternion slerp(tkFloat t, const Quaternion &q1, const Quaternion &q2) {
        tkFloat cosTheta = dot(q1, q2);

        // dot product threshold of 0.9995
        // if almost parallel in the same direction, linearly interpolate instead
        if (cosTheta > 0.9995f) {
            return normalize((1 - t) * q1 + t * q2);
        } else {
            // clamped for robustness because of possible FPE
            tkFloat theta0 = std::acos(clamp(cosTheta, -1, 1));
            tkFloat theta = theta0 * t;
            Quaternion qOrtho = normalize(q2 - q1 * cosTheta);
            return q1 * std::cos(theta) + qOrtho * std::sin(theta);
        }
    }
} // namespace TK