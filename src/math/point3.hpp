#pragma once

namespace TK {
    template <typename T>
    class Point3 {
    public:
        Point3() : x(0), y(0), z(0) {}
        Point3(T t) : x(t), y(t), z(t) {}
        Point3(T p0, T p1, T p2) : x(p0), y(p1), z(p2) {}
        Point3(const Point2<T>& xy, T z) : x(xy.x), y(xy.y), z(z) {}
        template <typename U>
        explicit Point3(const Point3<U>& p) : x((T)p.x), y((T)p.y), z((T)p.z) {}

        // Unary/subscript operators
        const Point3<T>& operator+() const;
        Point3<T> operator-() const;
        Point3<T> operator/(T w) const;
        T operator[](int i) const;
        T& operator[](int i);

        // Assignment operators
        Point3<T>& operator+=(const Vec3<T>& v);
        Point3<T>& operator-=(const Vec3<T>& v);

        // Equality
        bool operator==(const Point3<T>& p) const;
        bool operator!=(const Point3<T>& p) const;

        // Explicit cast
        explicit operator Point2<T>() const;
        explicit operator Vec2<T>() const;
        explicit operator Vec3<T>() const;

        std::string toString() const;

        union {
            struct {
                T x, y, z;
            };
            T entries[3];
        };

        static const Point3<T> zero;
    };

    template <typename T>
    inline const Point3<T> Point3<T>::zero = Point3<T>(0);

    template <typename T>
    inline const Point3<T>& Point3<T>::operator+() const {
        return *this;
    }

    template <typename T>
    inline Point3<T> Point3<T>::operator-() const {
        return Point3<T>(-x, -y, -z);
    }

    template <typename T>
    inline Point3<T> Point3<T>::operator/(T w) const {
        tkFloat k = 1.0 / w;
        return Point3<T>(x * k, y * k, z * k);
    }

    template <typename T>
    inline T Point3<T>::operator[](int i) const {
        return entries[i];
    }

    template <typename T>
    inline T& Point3<T>::operator[](int i) {
        return entries[i];
    }

    template <typename T>
    inline Point3<T>& Point3<T>::operator+=(const Vec3<T>& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    template <typename T>
    inline Point3<T>& Point3<T>::operator-=(const Vec3<T>& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template <typename T>
    inline bool Point3<T>::operator==(const Point3<T>& p) const {
        return x == p.x && y == p.y && z == p.z;
    }

    template <typename T>
    inline bool Point3<T>::operator!=(const Point3<T>& p) const {
        return x != p.x || y != p.y || z != p.z;
    }

    // Binary operators
    template <typename T>
    inline Point3<T> operator+(const Point3<T>& p1, const Point3<T>& p2) {
        return Point3<T>(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
    }

    template <typename T>
    inline Point3<T> operator+(const Point3<T>& p, const Vec3<T>& v) {
        return Point3<T>(p.x + v.x, p.y + v.y, p.z + v.z);
    }

    template <typename T>
    inline Vec3<T> operator-(const Point3<T>& p1, const Point3<T>& p2) {
        return Vec3<T>(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    }

    template <typename T>
    inline Point3<T> operator-(const Point3<T>& p, const Vec3<T>& v) {
        return Point3<T>(p.x - v.x, p.y - v.y, p.z - v.z);
    }

    template <typename T, typename U>
    inline Point3<T> operator*(const Point3<T>& p, U s) {
        return Point3(p.x * s, p.y * s, p.z * s);
    }

    template <typename T, typename U>
    inline Point3<T> operator*(U s, const Point3<T>& p) {
        return Point3<T>(p.x * s, p.y * s, p.z * s);
    }

    // Point operations
    template <typename T>
    inline tkFloat distance(const Point3<T>& p1, const Point3<T>& p2) {
        return (p2 - p1).magnitude();
    }

    template <typename T>
    inline tkFloat squaredDistance(const Point3<T>& p1, const Point3<T>& p2) {
        return (p2 - p1).squaredMagnitude();
    }

    template <typename T>
    inline Point3<T> swizzle(const Point3<T>& p, int x, int y, int z) {
        return Point3<T>(p[x], p[y], p[z]);
    }

    // Explicit cast
    template <typename T>
    inline Point3<T>::operator Point2<T>() const {
        return Point2<T>(x, y);
    }

    template <typename T>
    inline Point3<T>::operator Vec2<T>() const {
        return Vec2<T>(x, y);
    }

    template <typename T>
    inline Point3<T>::operator Vec3<T>() const {
        return Vec3<T>(x, y, z);
    }

    template <typename T>
    inline std::string Point3<T>::toString() const {
        std::ostringstream oss;
        oss << "Point3 [ " << x << ", " << y << ", " << z << " ]";
        return oss.str();
    }

    // IO stream operators
    template <typename T>
    inline std::istream& operator>>(std::istream& is, Point3<T>& p) {
        is >> p.x >> p.y >> p.z;
        return is;
    }

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Point3<T>& p) {
        os << p.toString() << "\n";
        return os;
    }
}  // namespace TK
