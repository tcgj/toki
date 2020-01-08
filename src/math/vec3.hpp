#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream>

class Vec3 {
public:
    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float f) : x(f), y(f), z(f) {}
    Vec3(float v0, float v1, float v2) : x(v0), y(v1), z(v2) {}
    Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}

    // Unary/subscript operators
    const Vec3& operator+() const;
    Vec3 operator-() const;
    float operator[](int i) const;
    float& operator[](int i);

    // Assignment operators
    Vec3& operator=(const Vec3& v);
    Vec3& operator+=(const Vec3& v);
    Vec3& operator-=(const Vec3& v);
    Vec3& operator*=(const Vec3& v);
    Vec3& operator*=(const float s);
    Vec3& operator/=(const Vec3& v);
    Vec3& operator/=(const float s);

    float magnitude() const;
    float squaredMagnitude() const;
    Vec3& normalize();

    // Explicit cast
    explicit operator Point3() const;

    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
        float data[3];
    };

    static const Vec3 zero;
    static const Vec3 one;
    static const Vec3 right;
    static const Vec3 forward;
};

const Vec3 Vec3::zero = Vec3(0.0f);
const Vec3 Vec3::one = Vec3(1.0f);
const Vec3 Vec3::right = Vec3(1.0f, 0.0f, 0.0f);
const Vec3 Vec3::forward = Vec3(0.0f, 0.0f, 1.0f);

inline const Vec3& Vec3::operator+() const {
    return *this;
}
inline Vec3 Vec3::operator-() const {
    return Vec3(-x, -y, -z);
}
inline float Vec3::operator[](int i) const {
    return data[i];
}
inline float& Vec3::operator[](int i) {
    return data[i];
}

inline Vec3& Vec3::operator=(const Vec3& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}
inline Vec3& Vec3::operator+=(const Vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
inline Vec3& Vec3::operator-=(const Vec3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
inline Vec3& Vec3::operator*=(const Vec3& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}
inline Vec3& Vec3::operator/=(const Vec3& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}
inline Vec3& Vec3::operator*=(float s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
}
inline Vec3& Vec3::operator/=(float s) {
    float k = 1.0f / s;
    x *= k;
    y *= k;
    z *= k;
    return *this;
}

inline float Vec3::magnitude() const {
    return sqrtf(x * x + y * y + z * z);
}
inline float Vec3::squaredMagnitude() const {
    return x * x + y * y + z * z;
}
inline Vec3& Vec3::normalize() {
    float sm = squaredMagnitude();
    if (sm > 0) {
        *this /= sqrtf(sm);
    }
    return *this;
}

// Additional operations
inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline Vec3 operator*(const Vec3& v, float s) {
    return Vec3(v.x * s, v.y * s, v.z * s);
}

inline Vec3 operator*(float s, const Vec3& v) {
    return Vec3(v.x * s, v.y * s, v.z * s);
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

inline Vec3 operator/(const Vec3& v, float s) {
    float k = 1.0f / s;
    return Vec3(v.x * k, v.y * k, v.z * k);
}

// Math
inline float dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
}
inline float angleBetween(const Vec3& v1, const Vec3& v2) {
    float cosTheta = dot(v1, v2) / (v1.magnitude() * v2.magnitude());
    return acosf(cosTheta);
}
inline Vec3 unitVectorOf(const Vec3& v) {
    float sm = v.squaredMagnitude();
    if (sm > 0) {
        return v / sqrtf(sm);
    } else {
        return v;
    }
}

// IO stream operators
inline std::istream& operator>>(std::istream& is, Vec3& v) {
    is >> v.x >> v.y >> v.z;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "[" << v.x << " " << v.y << " " << v.z << "]";
    return os;
}

#endif  // VECTOR3_HPP
