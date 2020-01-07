#include <cmath>
#include <iostream>

#ifndef VECTOR3_HPP
#define VECTOR3_HPP

class vec3 {
public:
    vec3() {}
    vec3(float v0, float v1, float v2) {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }

    // Getters
    inline float x() const { return v[0]; }
    inline float y() const { return v[1]; }
    inline float z() const { return v[2]; }
    inline float r() const { return v[0]; }
    inline float g() const { return v[1]; }
    inline float b() const { return v[2]; }

    // Setters
    inline float& x() { return v[0]; }
    inline float& y() { return v[1]; }
    inline float& z() { return v[2]; }
    inline float& r() { return v[0]; }
    inline float& g() { return v[1]; }
    inline float& b() { return v[2]; }

    // Unary/subscript operators
    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); }
    inline float operator[](int i) const { return v[i]; }
    inline float& operator[](int i) { return v[i]; }

    // Compound assignment operators
    inline vec3& operator+=(const vec3& o);
    inline vec3& operator-=(const vec3& o);
    inline vec3& operator*=(const vec3& o);
    inline vec3& operator*=(const float s);
    inline vec3& operator/=(const vec3& o);
    inline vec3& operator/=(const float s);

    inline float magnitude() const {
        return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    }
    inline float squared_magnitude() const {
        return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    }
    inline vec3& normalize() {
        float sm = squared_magnitude();
        if (sm > 0) {
            *this /= sqrtf(sm);
        }
        return *this;
    }

    // Binary operators
    friend vec3 operator+(const vec3& o1, const vec3& o2);
    friend vec3 operator-(const vec3& o1, const vec3& o2);
    friend vec3 operator*(const vec3& o1, const vec3& o2);
    friend vec3 operator*(const vec3& o, float s);
    friend vec3 operator*(float s, const vec3& o);
    friend vec3 operator/(const vec3& o1, const vec3& o2);
    friend vec3 operator/(const vec3& o, float s);
    friend float dot(const vec3& o1, const vec3& o2);
    friend vec3 cross(const vec3& o1, const vec3& o2);
    friend vec3 unit_vector_of(const vec3& o);

    static const vec3 zero;
    static const vec3 one;
private:
    float v[3];
};

const vec3 vec3::zero = vec3(0.0f, 0.0f, 0.0f);
const vec3 vec3::one = vec3(1.0f, 1.0f, 1.0f);

inline vec3& vec3::operator+=(const vec3& o) {
    v[0] += o.v[0];
    v[1] += o.v[1];
    v[2] += o.v[2];
    return *this;
}
inline vec3& vec3::operator-=(const vec3& o) {
    v[0] -= o.v[0];
    v[1] -= o.v[1];
    v[2] -= o.v[2];
    return *this;
}
inline vec3& vec3::operator*=(const vec3& o) {
    v[0] *= o.v[0];
    v[1] *= o.v[1];
    v[2] *= o.v[2];
    return *this;
}
inline vec3& vec3::operator/=(const vec3& o) {
    v[0] /= o.v[0];
    v[1] /= o.v[1];
    v[2] /= o.v[2];
    return *this;
}
inline vec3& vec3::operator*=(float s) {
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
    return *this;
}
inline vec3& vec3::operator/=(float s) {
    float k = 1.0f / s;
    v[0] *= k;
    v[1] *= k;
    v[2] *= k;
    return *this;
}

inline vec3 operator+(const vec3& o1, const vec3& o2) {
    return vec3(o1.v[0] + o2.v[0], o1.v[1] + o2.v[1], o1.v[2] + o2.v[2]);
}

inline vec3 operator-(const vec3& o1, const vec3& o2) {
    return vec3(o1.v[0] - o2.v[0], o1.v[1] - o2.v[1], o1.v[2] - o2.v[2]);
}

inline vec3 operator*(const vec3& o1, const vec3& o2) {
    return vec3(o1.v[0] * o2.v[0], o1.v[1] * o2.v[1], o1.v[2] * o2.v[2]);
}

inline vec3 operator*(const vec3& o, float s) {
    return vec3(o.v[0] * s, o.v[1] * s, o.v[2] * s);
}

inline vec3 operator*(float s, const vec3& o) {
    return vec3(o.v[0] * s, o.v[1] * s, o.v[2] * s);
}

inline vec3 operator/(const vec3& o1, const vec3& o2) {
    return vec3(o1.v[0] / o2.v[0], o1.v[1] / o2.v[1], o1.v[2] / o2.v[2]);
}

inline vec3 operator/(const vec3& o, float s) {
    float k = 1.0f / s;
    return vec3(o.v[0] * k, o.v[1] * k, o.v[2] * k);
}

inline float dot(const vec3& o1, const vec3& o2) {
    return o1.v[0] * o2.v[0] + o1.v[1] * o2.v[1] + o1.v[2] * o2.v[2];
}

inline vec3 cross(const vec3& o1, const vec3& o2) {
    return vec3(o1.v[1] * o2.v[2] - o1.v[2] * o2.v[1],
                o1.v[2] * o2.v[0] - o1.v[0] * o2.v[2],
                o1.v[0] * o2.v[1] - o1.v[1] * o2.v[0]);
}

inline vec3 unit_vector_of(const vec3& o) {
    float sm = o.squared_magnitude();
    if (sm > 0) {
        return o / sqrtf(sm);
    } else {
        return o;
    }
}

// IO stream operators
inline std::istream& operator>>(std::istream& is, vec3& o) {
    is >> o.x() >> o.y() >> o.z();
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const vec3& o) {
    os << "[" << o.x() << " " << o.y() << " " << o.z() << "]";
    return os;
}


#endif