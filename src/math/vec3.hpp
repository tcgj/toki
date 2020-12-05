#pragma once

namespace TK {
    template <typename T>
    class Vec3 {
    public:
        Vec3() : x(0), y(0), z(0) {}
        Vec3(T t) : x(t), y(t), z(t) {}
        Vec3(T v0, T v1, T v2) : x(v0), y(v1), z(v2) {}
        Vec3(const Vec2<T>& xy, T z) : x(xy.x), y(xy.y), z(z) {}
        template <typename U>
        explicit Vec3(const Vec3<U>& p) : x((T)p.x), y((T)p.y), z((T)p.z) {}

        // Unary/subscript operators
        const Vec3<T>& operator+() const;
        Vec3<T> operator-() const;
        T operator[](int i) const;
        T& operator[](int i);

        // Assignment operators
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
        explicit operator Point2<T>() const;
        explicit operator Point3<T>() const;
        explicit operator Vec2<T>() const;

        union {
            struct {
                T x, y, z;
            };
            struct {
                T r, g, b;
            };
            T entries[3];
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
    inline T Vec3<T>::operator[](int i) const {
        return entries[i];
    }
    template <typename T>
    inline T& Vec3<T>::operator[](int i) {
        return entries[i];
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
        tkFloat k = 1.0 / s;
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
        return std::sqrt(x * x + y * y + z * z);
    }
    template <typename T>
    inline tkFloat Vec3<T>::squaredMagnitude() const {
        return x * x + y * y + z * z;
    }
    template <typename T>
    inline Vec3<T>& Vec3<T>::normalized() {
        tkFloat sm = squaredMagnitude();
        if (sm > 0)
            *this /= std::sqrt(sm);

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
        return Vec3<T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
    }
    template <typename T>
    inline Vec3<T> operator/(const Vec3<T>& v, T s) {
        tkFloat k = 1.0 / s;
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
        return Vec3<T>(v1y * v2z - v1z * v2y, v1z * v2x - v1x * v2z, v1x * v2y - v1y * v2x);
    }
    template <typename T>
    inline Vec3<T> normalize(const Vec3<T>& v) {
        tkFloat sm = v.squaredMagnitude();
        if (sm > 0)
            return v / std::sqrt(sm);

        return v;
    }
    template <typename T>
    inline Vec3<T> swizzle(const Vec3<T>& v, int x, int y, int z) {
        return Vec3<T>(v[x], v[y], v[z]);
    }
    template <typename T>
    inline void coordinateSystem(const Vec3<T>& v1, Vec3<T>* v2, Vec3<T>* v3) {
        // v1 = z-axis, v2 = x-axis, v3 = y-axis
        if (std::abs(v1.x) > std::abs(v1.y))
            *v2 = Vec3<T>(v1.z, 0, -v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
        else
            *v2 = Vec3<T>(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
        // v3 is a unit vector only because v1 and v2 are orthogonal and normalized
        *v3 = cross(v1, *v2);
    }

    template <typename T>
    inline Vec3<T> reflect(const Vec3<T>& v, const Vec3<T>& n) {
        return v - 2 * dot(v, n) * n;
    }

    // Explicit cast
    template <typename T>
    inline Vec3<T>::operator Point2<T>() const {
        return Point2<T>(x, y);
    }
    template <typename T>
    inline Vec3<T>::operator Point3<T>() const {
        return Point3<T>(x, y, z);
    }
    template <typename T>
    inline Vec3<T>::operator Vec2<T>() const {
        return Vec2<T>(x, y);
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

}  // namespace TK
