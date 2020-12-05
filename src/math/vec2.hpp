#pragma once

namespace TK {
    template <typename T>
    class Vec2 {
    public:
        Vec2() : x(0), y(0) {}
        Vec2(T t) : x(t), y(t) {}
        Vec2(T v0, T v1) : x(v0), y(v1) {}
        template <typename U>
        explicit Vec2(const Vec2<U>& p) : x((T)p.x), y((T)p.y) {}

        // Unary/subscript operators
        const Vec2<T>& operator+() const;
        Vec2<T> operator-() const;
        T operator[](int i) const;
        T& operator[](int i);

        // Assignment operators
        Vec2<T>& operator+=(const Vec2<T>& v);
        Vec2<T>& operator-=(const Vec2<T>& v);
        Vec2<T>& operator*=(const Vec2<T>& v);
        Vec2<T>& operator*=(const T s);
        Vec2<T>& operator/=(const Vec2<T>& v);
        Vec2<T>& operator/=(const T s);

        // Equality
        bool operator==(const Vec2<T>& v) const;
        bool operator!=(const Vec2<T>& v) const;

        tkFloat magnitude() const;
        tkFloat squaredMagnitude() const;
        Vec2<T>& normalized();

        // Explicit cast
        explicit operator Point2<T>() const;
        explicit operator Vec3<T>() const;

        std::string toString() const;

        union {
            struct {
                T x, y;
            };
            struct {
                T s, t;
            };
            T entries[2];
        };

        static const Vec2<T> zero;
        static const Vec2<T> one;
    };

    template <typename T>
    inline const Vec2<T> Vec2<T>::zero = Vec2<T>(0);
    template <typename T>
    inline const Vec2<T> Vec2<T>::one = Vec2<T>(1);

    template <typename T>
    inline const Vec2<T>& Vec2<T>::operator+() const {
        return *this;
    }

    template <typename T>
    inline Vec2<T> Vec2<T>::operator-() const {
        return Vec2<T>(-x, -y);
    }

    template <typename T>
    inline T Vec2<T>::operator[](int i) const {
        return entries[i];
    }

    template <typename T>
    inline T& Vec2<T>::operator[](int i) {
        return entries[i];
    }

    template <typename T>
    inline Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    template <typename T>
    inline Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    template <typename T>
    inline Vec2<T>& Vec2<T>::operator*=(const Vec2<T>& v) {
        x *= v.x;
        y *= v.y;
        return *this;
    }

    template <typename T>
    inline Vec2<T>& Vec2<T>::operator/=(const Vec2<T>& v) {
        x /= v.x;
        y /= v.y;
        return *this;
    }

    template <typename T>
    inline Vec2<T>& Vec2<T>::operator*=(T s) {
        x *= s;
        y *= s;
        return *this;
    }

    template <typename T>
    inline Vec2<T>& Vec2<T>::operator/=(T s) {
        tkFloat k = 1.0 / s;
        x *= k;
        y *= k;
        return *this;
    }

    template <typename T>
    inline bool Vec2<T>::operator==(const Vec2<T>& v) const {
        return x == v.x && y == v.y;
    }

    template <typename T>
    inline bool Vec2<T>::operator!=(const Vec2<T>& v) const {
        return x != v.x || y != v.y;
    }

    template <typename T>
    inline tkFloat Vec2<T>::magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    template <typename T>
    inline tkFloat Vec2<T>::squaredMagnitude() const {
        return x * x + y * y;
    }

    template <typename T>
    inline Vec2<T>& Vec2<T>::normalized() {
        tkFloat sm = squaredMagnitude();
        if (sm > 0)
            *this /= std::sqrt(sm);

        return *this;
    }

    // Binary operators
    template <typename T>
    inline Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2) {
        return Vec2<T>(v1.x + v2.x, v1.y + v2.y);
    }

    template <typename T>
    inline Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2) {
        return Vec2<T>(v1.x - v2.x, v1.y - v2.y);
    }

    template <typename T>
    inline Vec2<T> operator*(const Vec2<T>& v1, const Vec2<T>& v2) {
        return Vec2<T>(v1.x * v2.x, v1.y * v2.y);
    }

    template <typename T>
    inline Vec2<T> operator*(const Vec2<T>& v, T s) {
        return Vec2<T>(v.x * s, v.y * s);
    }

    template <typename T>
    inline Vec2<T> operator*(T s, const Vec2<T>& v) {
        return Vec2<T>(v.x * s, v.y * s);
    }

    template <typename T>
    inline Vec2<T> operator/(const Vec2<T>& v1, const Vec2<T>& v2) {
        return Vec2<T>(v1.x / v2.x, v1.y / v2.y);
    }

    template <typename T>
    inline Vec2<T> operator/(const Vec2<T>& v, T s) {
        tkFloat k = 1.0 / s;
        return Vec3<T>(v.x * k, v.y * k);
    }

    // Vector operations
    template <typename T>
    inline Vec2<T> abs(const Vec2<T>& v) {
        return Vec2<T>(std::abs(v.x), std::abs(v.y));
    }

    template <typename T>
    inline tkFloat dot(const Vec2<T>& v1, const Vec2<T>& v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template <typename T>
    inline tkFloat cross(const Vec2<T>& v1, const Vec2<T>& v2) {
        return v1.x * v2.y - v1.y * v2.x;
    }

    template <typename T>
    inline Vec2<T> normalize(const Vec2<T>& v) {
        tkFloat sm = v.squaredMagnitude();
        if (sm > 0)
            return v / std::sqrt(sm);

        return v;
    }

    template <typename T>
    inline Vec2<T> swizzle(const Vec2<T>& v, int x, int y) {
        return Vec2<T>(v[x], v[y]);
    }

    template <typename T>
    inline Vec2<T> reflect(const Vec2<T>& v, const Vec2<T>& n) {
        return v - 2 * dot(v, n) * n;
    }

    // Explicit cast
    template <typename T>
    inline Vec2<T>::operator Point2<T>() const {
        return Point2<T>(x, y);
    }

    template <typename T>
    inline Vec2<T>::operator Vec3<T>() const {
        return Vec3<T>(x, y, 0);
    }

    template <typename T>
    inline std::string Vec2<T>::toString() const {
        std::ostringstream oss;
        oss << "Vector2 [ " << x << ", " << y << " ]";
        return oss.str();
    }

    // IO stream operators
    template <typename T>
    inline std::istream& operator>>(std::istream& is, Vec2<T>& v) {
        is >> v.x >> v.y;
        return is;
    }

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) {
        os << v.toString();
        return os;
    }

}  // namespace TK
