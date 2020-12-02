#pragma once

#include "stream.hpp"

namespace TK {
    class BufferedStream : public IOStream {
    public:
        BufferedStream(tkUInt initCapacity = 1024U) {
            if (initCapacity > 0)
                resize(initCapacity);
        }

        void resize(tkUInt size) {
            if (size <= capacity)
                return;

            std::unique_ptr<tkChar[]> newBuf = std::make_unique<tkChar[]>(size);
            memcpy(newBuf.get(), buf.get(), capacity);
            memset(newBuf.get() + capacity, 0, size - capacity);
            capacity = size;
            buf = std::move(newBuf);
        }

        // IStream operators
        BufferedStream& operator>>(tkFloat& f) override {
            if (rOffset + sizeof(f) > capacity)
                f = 0;
            else {
                memcpy(&f, buf.get() + rOffset, sizeof(f));
                rOffset += sizeof(f);
            }
            return *this;
        }

        BufferedStream& operator>>(tkInt& i) override {
            if (rOffset + sizeof(i) > capacity)
                i = 0;
            else {
                memcpy(&i, buf.get() + rOffset, sizeof(i));
                rOffset += sizeof(i);
            }
            return *this;
        }

        BufferedStream& operator>>(tkUInt& u) override {
            if (rOffset + sizeof(u) > capacity)
                u = 0;
            else {
                memcpy(&u, buf.get() + rOffset, sizeof(u));
                rOffset += sizeof(u);
            }
            return *this;
        }

        BufferedStream& operator>>(std::string& s) override {
            s = "";
            tkChar c;
            while (rOffset < capacity) {
                c = buf[rOffset++];
                if (c == 0)
                    break;
                s += c;
            }
            return *this;
        }

        BufferedStream& operator>>(bool& b) override {
            if (rOffset + sizeof(b) > capacity)
                b = 0;
            else {
                memcpy(&b, buf.get() + rOffset, sizeof(b));
                rOffset += sizeof(b);
            }
            return *this;
        }

        BufferedStream& read(tkChar* data, tkUInt size) override {
            if (rOffset + size > capacity)
                memset(data, 0, size);
            else {
                memcpy(data, buf.get() + rOffset, size);
                rOffset += size;
            }
            return *this;
        }

        // OStream operators
        BufferedStream& operator<<(tkFloat f) override {
            if (wOffset + sizeof(f) > capacity) {
                resize(2 * capacity);
                return *this << f;
            }

            memcpy(buf.get() + wOffset, &f, sizeof(f));
            wOffset += sizeof(f);
            return *this;
        }

        BufferedStream& operator<<(tkInt i) override {
            if (wOffset + sizeof(i) > capacity) {
                resize(2 * capacity);
                return *this << i;
            }

            memcpy(buf.get() + wOffset, &i, sizeof(i));
            wOffset += sizeof(i);
            return *this;
        }

        BufferedStream& operator<<(tkUInt u) override {
            if (wOffset + sizeof(u) > capacity) {
                resize(2 * capacity);
                return *this << u;
            }

            memcpy(buf.get() + wOffset, &u, sizeof(u));
            wOffset += sizeof(u);
            return *this;
        }

        BufferedStream& operator<<(const std::string& s) override {
            size_t charCount = s.size() + 1;
            if (wOffset + charCount > capacity) {
                resize(2 * capacity);
                return *this << s;
            }

            memcpy(buf.get() + wOffset, s.c_str(), charCount);
            wOffset += charCount;
            return *this;
        }

        BufferedStream& operator<<(bool b) override {
            if (wOffset + sizeof(b) > capacity) {
                resize(2 * capacity);
                return *this << b;
            }

            memcpy(buf.get() + wOffset, &b, sizeof(b));
            wOffset += sizeof(b);
            return *this;
        }

        BufferedStream& write(const tkChar* data, tkUInt size) override {
            if (wOffset + size > capacity) {
                resize(2 * capacity);
                return write(data, size);
            }

            memcpy(buf.get() + wOffset, &data, size);
            wOffset += size;
            return *this;
        }

    private:
        tkUInt rOffset;
        tkUInt wOffset;
        tkUInt capacity;
        std::unique_ptr<tkChar[]> buf;
    };
}  // namespace TK
