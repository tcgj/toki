#pragma once

namespace TK {
    template <typename T>
    class Point2 {
    public:
        Point2() : x(0), y(0) {}
        Point2(T t) : x(t), y(t) {}
        Point2(T p0, T p1) : x(p0), y(p1) {}
        template <typename U>
        explicit Point2(const Point2<U>& p) : x((T)p.x), y((T)p.y) {}

        // Unary/subscript operators
        const Point2<T>& operator+() const;
        Point2<T> operator-() const;
        Point2<T> operator/(T w) const;
        T operator[](int i) const;
        T& operator[](int i);

        // Assignment operators
        Point2<T>& operator+=(const Vec2<T>& v);
        Point2<T>& operator-=(const Vec2<T>& v);

        // Equality
        bool operator==(const Point2<T>& p) const;
        bool operator!=(const Point2<T>& p) const;

        // Explicit cast
        explicit operator Vec2<T>() const;
        explicit operator Point3<T>() const;

        union {
            struct {
                T x, y;
            };
            struct {
                T s, t;
            };
            T entries[2];
        };

        static const Point2<T> zero;
    };

    template <typename T>
    inline const Point2<T> Point2<T>::zero = Point2<T>(0);

    template <typename T>
    inline const Point2<T>& Point2<T>::operator+() const {
        return *this;
    }
    template <typename T>
    inline Point2<T> Point2<T>::operator-() const {
        return Point2<T>(-x, -y);
    }
    template <typename T>
    inline Point2<T> Point2<T>::operator/(T w) const {
        tkFloat k = 1.0 / w;
        return Point2<T>(x * k, y * k);
    }
    template <typename T>
    inline T Point2<T>::operator[](int i) const {
        return entries[i];
    }
    template <typename T>
    inline T& Point2<T>::operator[](int i) {
        return entries[i];
    }

    template <typename T>
    inline Point2<T>& Point2<T>::operator+=(const Vec2<T>& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    template <typename T>
    inline Point2<T>& Point2<T>::operator-=(const Vec2<T>& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    template <typename T>
    inline bool Point2<T>::operator==(const Point2<T>& p) const {
        return x == p.x && y == p.y;
    }
    template <typename T>
    inline bool Point2<T>::operator!=(const Point2<T>& p) const {
        return x != p.x || y != p.y;
    }

    // Binary operators
    template <typename T>
    inline Point2<T> operator+(const Point2<T>& p1, const Point2<T>& p2) {
        return Point2<T>(p1.x + p2.x, p1.y + p2.y);
    }
    template <typename T>
    inline Point2<T> operator+(const Point2<T>& p, const Vec2<T>& v) {
        return Point2<T>(p.x + v.x, p.y + v.y);
    }
    template <typename T>
    inline Vec2<T> operator-(const Point2<T>& p1, const Point2<T>& p2) {
        return Vec2<T>(p1.x - p2.x, p1.y - p2.y);
    }
    template <typename T>
    inline Point2<T> operator-(const Point2<T>& p, const Vec2<T>& v) {
        return Point2<T>(p.x - v.x, p.y - v.y);
    }
    template <typename T, typename U>
    inline Point2<T> operator*(const Point2<T>& p, U s) {
        return Point2(p.x * s, p.y * s);
    }
    template <typename T, typename U>
    inline Point2<T> operator*(U s, const Point2<T>& p) {
        return Point2<T>(p.x * s, p.y * s);
    }

    // Point operations
    template <typename T>
    inline tkFloat distance(const Point2<T>& p1, const Point2<T>& p2) {
        return (p2 - p1).magnitude();
    }
    template <typename T>
    inline tkFloat squaredDistance(const Point2<T>& p1, const Point2<T>& p2) {
        return (p2 - p1).squaredMagnitude();
    }
    template <typename T>
    inline Point2<T> swizzle(const Point2<T>& p, int x, int y) {
        return Point2<T>(p[x], p[y]);
    }

    // Explicit cast
    template <typename T>
    inline Point2<T>::operator Point3<T>() const {
        return Point3<T>(x, y, 0);
    }
    template <typename T>
    inline Point2<T>::operator Vec2<T>() const {
        return Vec2<T>(x, y);
    }

    // IO stream operators
    template <typename T>
    inline std::istream& operator>>(std::istream& is, Point2<T>& p) {
        is >> p.x >> p.y;
        return is;
    }

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Point2<T>& p) {
        os << "[" << p.x << " " << p.y << "]";
        return os;
    }
}  // namespace TK
