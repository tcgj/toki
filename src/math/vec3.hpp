#pragma once

#include <cmath>
#include <iostream>

namespace TK {
    template <typename T>
    class Vec3 {
    public:
        Vec3() : x(0), y(0), z(0) {}
        Vec3(T t) : x(t), y(t), z(t) {}
        Vec3(T v0, T v1, T v2) : x(v0), y(v1), z(v2) {
            tkAssert(std::isnan(x) || std::isnan(y) || std::isnan(z));
        }
        Vec3(const Vec3<T>& v) : x(v.x), y(v.y), z(v.z) {}

        // Unary/subscript operators
        const Vec3<T>& operator+() const;
        Vec3<T> operator-() const;
        T operator[](tkInt i) const;
        T& operator[](tkInt i);

        // Assignment operators
        Vec3<T>& operator=(const Vec3<T>& v);
        Vec3<T>& operator+=(const Vec3<T>& v);
        Vec3<T>& operator-=(const Vec3<T>& v);
        Vec3<T>& operator*=(const Vec3<T>& v);
        Vec3<T>& operator*=(const T s);
        Vec3<T>& operator/=(const Vec3<T>& v);
        Vec3<T>& operator/=(const T s);

        // Equality
        bool operator==(const Vec3<T>& v) const;
        bool operator!=(const Vec3<T>& v) const;

        tkFloat magnitude() const;
        tkFloat squaredMagnitude() const;
        Vec3<T>& normalized();

        // Explicit cast
        explicit operator Point3<T>() const;

        union {
            struct { T x, y, z; };
            struct { T r, g, b; };
            T data[3];
        };

        static const Vec3<T> zero;
        static const Vec3<T> one;
        static const Vec3<T> right;
        static const Vec3<T> forward;
    };

    template <typename T>
    inline const Vec3<T> Vec3<T>::zero = Vec3<T>(0);
    template <typename T>
    inline const Vec3<T> Vec3<T>::one = Vec3<T>(1);
    template <typename T>
    inline const Vec3<T> Vec3<T>::right = Vec3<T>(1, 0, 0);
    template <typename T>
    inline const Vec3<T> Vec3<T>::forward = Vec3<T>(0, 0, 1);

    template <typename T>
    inline const Vec3<T>& Vec3<T>::operator+() const {
        return *this;
    }
    template <typename T>
    inline Vec3<T> Vec3<T>::operator-() const {
        return Vec3<T>(-x, -y, -z);
    }
    template <typename T>
    inline T Vec3<T>::operator[](tkInt i) const {
        tkAssert(i >= 0 && i <= 2);
        return data[i];
    }
    template <typename T>
    inline T& Vec3<T>::operator[](tkInt i) {
        tkAssert(i >= 0 && i <= 2);
        return data[i];
    }

    template <typename T>
    inline Vec3<T>& Vec3<T>::operator=(const Vec3<T>& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
    template <typename T>
    inline Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    template <typename T>
    inline Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    template <typename T>
    inline Vec3<T>& Vec3<T>::operator*=(const Vec3<T>& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    template <typename T>
    inline Vec3<T>& Vec3<T>::operator/=(const Vec3<T>& v) {
        tkAssert(std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z));
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    template <typename T>
    inline Vec3<T>& Vec3<T>::operator*=(T s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    template <typename T>
    inline Vec3<T>& Vec3<T>::operator/=(T s) {
        tkAssert(s != 0);
        tkFloat k = (tkFloat)1 / s;
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }

    template <typename T>
    inline bool Vec3<T>::operator==(const Vec3<T>& v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    template <typename T>
    inline bool Vec3<T>::operator!=(const Vec3<T>& v) const {
        return x != v.x || y != v.y || z != v.z;
    }

    template <typename T>
    inline tkFloat Vec3<T>::magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }
    template <typename T>
    inline tkFloat Vec3<T>::squaredMagnitude() const {
        return x * x + y * y + z * z;
    }
    template <typename T>
    inline Vec3<T>& Vec3<T>::normalized() {
        tkFloat sm = squaredMagnitude();
        if (sm > 0) {
            *this /= sqrt(sm);
        }
        return *this;
    }

    // Binary operators
    template <typename T>
    inline Vec3<T> operator+(const Vec3<T>& v1, const Vec3<T>& v2) {
        return Vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }
    template <typename T>
    inline Vec3<T> operator-(const Vec3<T>& v1, const Vec3<T>& v2) {
        return Vec3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }
    template <typename T>
    inline Vec3<T> operator*(const Vec3<T>& v1, const Vec3<T>& v2) {
        return Vec3<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
    }
    template <typename T>
    inline Vec3<T> operator*(const Vec3<T>& v, T s) {
        return Vec3<T>(v.x * s, v.y * s, v.z * s);
    }
    template <typename T>
    inline Vec3<T> operator*(T s, const Vec3<T>& v) {
        return Vec3<T>(v.x * s, v.y * s, v.z * s);
    }
    template <typename T>
    inline Vec3<T> operator/(const Vec3<T>& v1, const Vec3<T>& v2) {
        tkAssert(std::isnan(v2.x) || std::isnan(v2.y) || std::isnan(v2.z));
        return Vec3<T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
    }
    template <typename T>
    inline Vec3<T> operator/(const Vec3<T>& v, T s) {
        tkAssert(s != 0);
        tkFloat k = (tkFloat)1 / s;
        return Vec3<T>(v.x * k, v.y * k, v.z * k);
    }

    // Vector operations
    template <typename T>
    inline Vec3<T> abs(const Vec3<T>& v) {
        return Vec3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
    }
    template <typename T>
    inline tkFloat dot(const Vec3<T>& v1, const Vec3<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
    template <typename T>
    inline Vec3<T> cross(const Vec3<T>& v1, const Vec3<T>& v2) {
        double v1x = v1.x, v1y = v1.y, v1z = v1.z;
        double v2x = v2.x, v2y = v2.y, v2z = v2.z;
        return Vec3<T>(v1y * v2z - v1z * v2y, v1z * v2x - v1x * v2z,
                    v1x * v2y - v1y * v2x);
    }
    template <typename T>
    inline tkFloat angleBetween(const Vec3<T>& v1, const Vec3<T>& v2) {
        tkFloat cosTheta = dot(v1, v2) / (v1.magnitude() * v2.magnitude());
        return acos(cosTheta);
    }
    template <typename T>
    inline Vec3<T> normalize(const Vec3<T>& v) {
        tkFloat sm = v.squaredMagnitude();
        if (sm > 0) {
            return v / sqrt(sm);
        }
        return v;
    }
    template <typename T>
    inline Vec3<T> swizzle(const Vec3<T>& v, int x, int y, int z) {
        return Vec3<T>(v[x], v[y], v[z]);
    }
    template <typename T>
    inline void orthogonalAxes(const Vec3<T>& v1, Vec3<T>* v2, Vec3<T>* v3) {
        if (std::abs(v1.x) > std::abs(v1.y))
            *v2 = Vec3<T>(-v1.z, 0, v1.x) / sqrt(v1.x * v1.x + v1.z * v1.z);
        else
            *v2 = Vec3<T>(0, v1.z, -v1.y) / sqrt(v1.y * v1.y + v1.z * v1.z);
        *v3 = cross(v1, *v2);
    }

    template <typename T>
    inline Vec3<T> reflect(const Vec3<T>& v, const Vec3<T>& n) {
        return v - 2 * dot(v, n) * n;
    }

    // IO stream operators
    template <typename T>
    inline std::istream& operator>>(std::istream& is, Vec3<T>& v) {
        is >> v.x >> v.y >> v.z;
        return is;
    }

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vec3<T>& v) {
        os << "[" << v.x << " " << v.y << " " << v.z << "]";
        return os;
    }

} // namespace TK
