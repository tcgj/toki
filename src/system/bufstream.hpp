#pragma once

#include "stream.hpp"

namespace TK {
    class BufferedStream : public IOStream {
    public:
        BufferedStream(unsigned int initCapacity = 1024U) {
            if (initCapacity > 0)
                resize(initCapacity);
        }

        void resize(unsigned int size) {
            if (size <= m_Capacity)
                return;

            std::unique_ptr<char[]> newBuf = std::make_unique<char[]>(size);
            memcpy(newBuf.get(), m_Buffer.get(), m_Capacity);
            memset(newBuf.get() + m_Capacity, 0, size - m_Capacity);
            m_Capacity = size;
            m_Buffer = std::move(newBuf);
        }

        // IStream operators
        BufferedStream& operator>>(tkFloat& f) override {
            if (m_ReadOffset + sizeof(f) > m_Capacity)
                f = 0;
            else {
                memcpy(&f, m_Buffer.get() + m_ReadOffset, sizeof(f));
                m_ReadOffset += sizeof(f);
            }
            return *this;
        }

        BufferedStream& operator>>(int& i) override {
            if (m_ReadOffset + sizeof(i) > m_Capacity)
                i = 0;
            else {
                memcpy(&i, m_Buffer.get() + m_ReadOffset, sizeof(i));
                m_ReadOffset += sizeof(i);
            }
            return *this;
        }

        BufferedStream& operator>>(unsigned int& u) override {
            if (m_ReadOffset + sizeof(u) > m_Capacity)
                u = 0;
            else {
                memcpy(&u, m_Buffer.get() + m_ReadOffset, sizeof(u));
                m_ReadOffset += sizeof(u);
            }
            return *this;
        }

        BufferedStream& operator>>(std::string& s) override {
            s = "";
            char c;
            while (m_ReadOffset < m_Capacity) {
                c = m_Buffer[m_ReadOffset++];
                if (c == 0)
                    break;
                s += c;
            }
            return *this;
        }

        BufferedStream& operator>>(bool& b) override {
            if (m_ReadOffset + sizeof(b) > m_Capacity)
                b = 0;
            else {
                memcpy(&b, m_Buffer.get() + m_ReadOffset, sizeof(b));
                m_ReadOffset += sizeof(b);
            }
            return *this;
        }

        BufferedStream& read(char* data, unsigned int size) override {
            if (m_ReadOffset + size > m_Capacity)
                memset(data, 0, size);
            else {
                memcpy(data, m_Buffer.get() + m_ReadOffset, size);
                m_ReadOffset += size;
            }
            return *this;
        }

        // OStream operators
        BufferedStream& operator<<(tkFloat f) override {
            if (m_WriteOffset + sizeof(f) > m_Capacity) {
                resize(2 * m_Capacity);
                return *this << f;
            }

            memcpy(m_Buffer.get() + m_WriteOffset, &f, sizeof(f));
            m_WriteOffset += sizeof(f);
            return *this;
        }

        BufferedStream& operator<<(int i) override {
            if (m_WriteOffset + sizeof(i) > m_Capacity) {
                resize(2 * m_Capacity);
                return *this << i;
            }

            memcpy(m_Buffer.get() + m_WriteOffset, &i, sizeof(i));
            m_WriteOffset += sizeof(i);
            return *this;
        }

        BufferedStream& operator<<(unsigned int u) override {
            if (m_WriteOffset + sizeof(u) > m_Capacity) {
                resize(2 * m_Capacity);
                return *this << u;
            }

            memcpy(m_Buffer.get() + m_WriteOffset, &u, sizeof(u));
            m_WriteOffset += sizeof(u);
            return *this;
        }

        BufferedStream& operator<<(const std::string& s) override {
            size_t charCount = s.size() + 1;
            if (m_WriteOffset + charCount > m_Capacity) {
                resize(2 * m_Capacity);
                return *this << s;
            }

            memcpy(m_Buffer.get() + m_WriteOffset, s.c_str(), charCount);
            m_WriteOffset += charCount;
            return *this;
        }

        BufferedStream& operator<<(bool b) override {
            if (m_WriteOffset + sizeof(b) > m_Capacity) {
                resize(2 * m_Capacity);
                return *this << b;
            }

            memcpy(m_Buffer.get() + m_WriteOffset, &b, sizeof(b));
            m_WriteOffset += sizeof(b);
            return *this;
        }

        BufferedStream& write(const char* data, unsigned int size) override {
            if (m_WriteOffset + size > m_Capacity) {
                resize(2 * m_Capacity);
                return write(data, size);
            }

            memcpy(m_Buffer.get() + m_WriteOffset, &data, size);
            m_WriteOffset += size;
            return *this;
        }

    private:
        unsigned int m_ReadOffset;
        unsigned int m_WriteOffset;
        unsigned int m_Capacity;
        std::unique_ptr<char[]> m_Buffer;
    };
}  // namespace TK
