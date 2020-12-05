#pragma once

#include "system/toki.hpp"
#include "core/transform.hpp"
#include "core/spectrum.hpp"

namespace TK {
    class IStream {
    public:
        // Interface for primitive type output streaming
        virtual IStream& operator>>(tkFloat& f) = 0;
        virtual IStream& operator>>(int& i) = 0;
        virtual IStream& operator>>(unsigned int& u) = 0;
        virtual IStream& operator>>(std::string& s) = 0;
        virtual IStream& operator>>(bool& b) = 0;
        virtual IStream& read(char* data, unsigned int size) = 0;

        // Pre-defined output streaming operators
        IStream& operator>>(Vec2f& v) {
            return *this >> v.x >> v.y;
        }

        IStream& operator>>(Vec3f& v) {
            return *this >> v.x >> v.y >> v.z;
        }

        IStream& operator>>(Vec2i& v) {
            return *this >> v.x >> v.y;
        }

        IStream& operator>>(Vec3i& v) {
            return *this >> v.x >> v.y >> v.z;
        }

        IStream& operator>>(Point2f& p) {
            return *this >> p.x >> p.y;
        }

        IStream& operator>>(Point3f& p) {
            return *this >> p.x >> p.y >> p.z;
        }

        IStream& operator>>(Point2i& p) {
            return *this >> p.x >> p.y;
        }

        IStream& operator>>(Point3i& p) {
            return *this >> p.x >> p.y >> p.z;
        }

        IStream& operator>>(Matrix44& m) {
            for (int i = 0; i < 16; ++i) {
                *this >> m.m_Entries[i];
            }
            return *this;
        }

        IStream& operator>>(Transform& t) {
            Matrix44 m;
            *this >> m;
            t = Transform(m);
            return *this;
        }

        IStream& operator>>(tkSpectrum& s) {
            Vec3f v;
            *this >> v.r >> v.g >> v.b;
            s = tkSpectrum::fromRGB(v);
            return *this;
        }
    };

    class OStream {
    public:
        // Interface for primitive type output streaming
        virtual OStream& operator<<(tkFloat f) = 0;
        virtual OStream& operator<<(int i) = 0;
        virtual OStream& operator<<(unsigned int u) = 0;
        virtual OStream& operator<<(const std::string& s) = 0;
        virtual OStream& operator<<(bool b) = 0;
        virtual OStream& write(const char* data, unsigned int size) = 0;
        virtual void flush() {}

        // Pre-defined output streaming operators
        OStream& operator<<(const Vec2f& v) {
            return *this << v.x << v.y;
        }

        OStream& operator<<(const Vec3f& v) {
            return *this << v.x << v.y << v.z;
        }

        OStream& operator<<(const Vec2i& v) {
            return *this << v.x << v.y;
        }

        OStream& operator<<(const Vec3i& v) {
            return *this << v.x << v.y << v.z;
        }

        OStream& operator<<(const Point2f& p) {
            return *this << p.x << p.y;
        }

        OStream& operator<<(const Point3f& p) {
            return *this << p.x << p.y << p.z;
        }

        OStream& operator<<(const Point2i& p) {
            return *this << p.x << p.y;
        }

        OStream& operator<<(const Point3i& p) {
            return *this << p.x << p.y << p.z;
        }

        OStream& operator<<(const Matrix44& m) {
            for (int i = 0; i < 16; ++i)
                *this << m.m_Entries[i];

            return *this;
        }

        OStream& operator<<(const Transform& t) {
            return *this << t.getMatrix();
        }

        OStream& operator<<(const tkSpectrum& s) {
            return *this << s.toRGB();
        }
    };

    class IOStream : public IStream, public OStream {};
}  // namespace TK
