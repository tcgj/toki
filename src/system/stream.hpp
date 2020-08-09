#pragma once

#include "math/math.hpp"
#include "core/transform.hpp"
#include "core/spectrum.hpp"

namespace TK {
    class IStream {
    public:
        // Interface for primitive type output streaming
        virtual IStream &operator>>(tkFloat &f) = 0;
        virtual IStream &operator>>(tkInt &i) = 0;
        virtual IStream &operator>>(tkUInt &u) = 0;
        virtual IStream &operator>>(std::string &s) = 0;
        virtual IStream &operator>>(bool &b) = 0;
        virtual IStream &read(tkChar *data, tkUInt size) = 0;

        // Pre-defined output streaming operators
        IStream &operator>>(tkVec2f &v) {
            return *this >> v.x >> v.y;
        }

        IStream &operator>>(tkVec3f &v) {
            return *this >> v.x >> v.y >> v.z;
        }

        IStream &operator>>(tkVec2i &v) {
            return *this >> v.x >> v.y;
        }

        IStream &operator>>(tkVec3i &v) {
            return *this >> v.x >> v.y >> v.z;
        }

        IStream &operator>>(tkPoint2f &p) {
            return *this >> p.x >> p.y;
        }

        IStream &operator>>(tkPoint3f &p) {
            return *this >> p.x >> p.y >> p.z;
        }

        IStream &operator>>(tkPoint2i &p) {
            return *this >> p.x >> p.y;
        }

        IStream &operator>>(tkPoint3i &p) {
            return *this >> p.x >> p.y >> p.z;
        }

        IStream &operator>>(Matrix44 &m) {
            for (tkInt i = 0; i < 16; ++i) {
                *this >> m.entries[i];
            }
            return *this;
        }

        IStream &operator>>(Transform &t) {
            Matrix44 m;
            *this >> m;
            t = Transform(m);
            return *this;
        }

        IStream &operator>>(tkSpectrum &s) {
            tkVec3f v;
            *this >> v.r >> v.g >> v.b;
            s = tkSpectrum::fromRGB(v);
            return *this;
        }
    };

    class OStream {
    public:
        // Interface for primitive type output streaming
        virtual OStream &operator<<(tkFloat f) = 0;
        virtual OStream &operator<<(tkInt i) = 0;
        virtual OStream &operator<<(tkUInt u) = 0;
        virtual OStream &operator<<(const std::string &s) = 0;
        virtual OStream &operator<<(bool b) = 0;
        virtual OStream &write(const tkChar *data, tkUInt size) = 0;
        virtual void flush() {}

        // Pre-defined output streaming operators
        OStream &operator<<(const tkVec2f &v) {
            return *this << v.x << v.y;
        }

        OStream &operator<<(const tkVec3f &v) {
            return *this << v.x << v.y << v.z;
        }

        OStream &operator<<(const tkVec2i &v) {
            return *this << v.x << v.y;
        }

        OStream &operator<<(const tkVec3i &v) {
            return *this << v.x << v.y << v.z;
        }

        OStream &operator<<(const tkPoint2f &p) {
            return *this << p.x << p.y;
        }

        OStream &operator<<(const tkPoint3f &p) {
            return *this << p.x << p.y << p.z;
        }

        OStream &operator<<(const tkPoint2i &p) {
            return *this << p.x << p.y;
        }

        OStream &operator<<(const tkPoint3i &p) {
            return *this << p.x << p.y << p.z;
        }

        OStream &operator<<(const Matrix44 &m) {
            for (tkInt i = 0; i < 16; ++i) {
                *this << m.entries[i];
            }
            return *this;
        }

        OStream &operator<<(const Transform &t) {
            return *this << t.getMatrix();
        }

        OStream &operator<<(const tkSpectrum &s) {
            return *this << s.toRGB();
        }
    };

    class IOStream : public IStream, public OStream {};
} // namespace TK
