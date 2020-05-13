#pragma once

namespace TK {
    template <typename T>
    class Point3 {
    public:
        Point3() : x(0), y(0), z(0) {}
        Point3(T t) : x(t), y(t), z(t) {}
        Point3(T p0, T p1, T p2) : x(p0), y(p1), z(p2) {
            tkAssert(std::isnan(x) || std::isnan(y) || std::isnan(z));
        }
        Point3(const Point3<T> &p) : x(p.x), y(p.y), z(p.z) {}

        // Unary/subscript operators
        const Point3<T> &operator+() const;
        Point3<T> operator-() const;
        Point3<T> operator/(T w) const;
        T operator[](tkInt i) const;
        T &operator[](tkInt i);

        // Assignment operators
        Point3<T> &operator=(const Point3<T> &p);
        Point3<T> &operator+=(const Vec3<T> &v);
        Point3<T> &operator-=(const Vec3<T> &v);

        // Equality
        bool operator==(const Point3<T> &p) const;
        bool operator!=(const Point3<T> &p) const;

        // Explicit cast
        explicit operator Vec3<T>() const;

        union {
            struct { T x, y, z; };
            tkFloat data[3];
        };

        static const Point3<T> zero;
    };

    template <typename T>
    inline const Point3<T> Point3<T>::zero = Point3<T>(0);

    template <typename T>
    inline const Point3<T> &Point3<T>::operator+() const {
        return *this;
    }
    template <typename T>
    inline Point3<T> Point3<T>::operator-() const {
        return Point3<T>(-x, -y, -z);
    }
    template <typename T>
    inline Point3<T> Point3<T>::operator/(T w) const {
        tkAssert(w != 0);
        tkFloat k = (tkFloat)1 / w;
        return Point3<T>(x * k, y * k, z * k);
    }
    template <typename T>
    inline T Point3<T>::operator[](tkInt i) const {
        tkAssert(i >= 0 && i <= 2);
        return data[i];
    }
    template <typename T>
    inline T &Point3<T>::operator[](tkInt i) {
        tkAssert(i >= 0 && i <= 2);
        return data[i];
    }

    template <typename T>
    inline Point3<T> &Point3<T>::operator=(const Point3<T> &p) {
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }
    template <typename T>
    inline Point3<T> &Point3<T>::operator+=(const Vec3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    template <typename T>
    inline Point3<T> &Point3<T>::operator-=(const Vec3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template <typename T>
    inline bool Point3<T>::operator==(const Point3<T> &p) const {
        return x == p.x && y == p.y && z == p.z;
    }
    template <typename T>
    inline bool Point3<T>::operator!=(const Point3<T> &p) const {
        return x != p.x || y != p.y || z != p.z;
    }

    // Binary operators
    template <typename T>
    inline Point3<T> operator*(const Point3<T> &p, T s) {
        return Point3(p.x * s, p.y * s, p.z * s);
    }
    template <typename T>
    inline Point3<T> operator*(T s, const Point3<T> &p) {
        return Point3<T>(p.x * s, p.y * s, p.z * s);
    }
    template <typename T>
    inline Vec3<T> operator-(const Point3<T> &p1, const Point3<T> &p2) {
        return Vec3<T>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    }

    // Point operations
    template <typename T>
    inline tkFloat distance(const Point3<T> &p1, const Point3<T> &p2) {
        return (p2 - p1).magnitude();
    }
    template <typename T>
    inline tkFloat squaredDistance(const Point3<T> &p1, const Point3<T> &p2) {
        return (p2 - p1).squaredMagnitude();
    }
    template <typename T>
    inline Point3<T> swizzle(const Point3<T> &p, tkInt x, tkInt y, tkInt z) {
        return Point3<T>(p[x], p[y], p[z]);
    }
} // namespace TK
