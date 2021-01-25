#pragma once

#include <fstream>

#include "stream.hpp"

namespace TK {
    enum FileStreamMode {
        FSM_IN = std::ios_base::in | std::ios_base::binary,
        FSM_OUT = std::ios_base::out | std::ios_base::binary
    };

    class FileStream : public IOStream {
    public:
        FileStream() = default;

        FileStream(const std::string filename, FileStreamMode fsm) : m_File(std::fstream(filename, fsm)) {
            // TODO: Setup a proper check for fail
        }

        bool open(std::string filename, FileStreamMode fsm, bool forceOpen = false) {
            if (m_File.is_open()) {
                if (forceOpen)
                    m_File.close();
                else
                    return false;
            }
            m_File.open(filename, fsm);
            return m_File.is_open();
        }

        bool close() {
            if (!m_File.is_open())
                return false;
            m_File.close();
            return true;
        }

        // IStream operators
        FileStream& operator>>(tkFloat& f) override {
            m_File.read(reinterpret_cast<char*>(&f), sizeof(f));
            return *this;
        }

        FileStream& operator>>(int& i) override {
            m_File.read(reinterpret_cast<char*>(&i), sizeof(i));
            return *this;
        }

        FileStream& operator>>(unsigned int& u) override {
            m_File.read(reinterpret_cast<char*>(&u), sizeof(u));
            return *this;
        }

        FileStream& operator>>(std::string& s) override {
            s = "";
            char c;
            do {
                m_File.read(&c, sizeof(c));
                if (c == 0)
                    break;
                s += c;
            } while (m_File.good());
            return *this;
        }

        FileStream& operator>>(bool& b) override {
            m_File.read(reinterpret_cast<char*>(&b), sizeof(b));
            return *this;
        }

        FileStream& read(char* data, unsigned int size) override {
            m_File.read(data, size);
            return *this;
        }

        // OStream operators
        FileStream& operator<<(tkFloat f) override {
            m_File.write(reinterpret_cast<const char*>(&f), sizeof(f));
            return *this;
        }

        FileStream& operator<<(int i) override {
            m_File.write(reinterpret_cast<const char*>(&i), sizeof(i));
            return *this;
        }

        FileStream& operator<<(unsigned int u) override {
            m_File.write(reinterpret_cast<const char*>(&u), sizeof(u));
            return *this;
        }

        FileStream& operator<<(const std::string& s) override {
            m_File.write(s.c_str(), s.size() + 1);
            return *this;
        }

        FileStream& operator<<(bool b) override {
            m_File.write(reinterpret_cast<const char*>(&b), sizeof(b));
            return *this;
        }

        FileStream& write(const char* data, unsigned int size) override {
            m_File.write(data, size);
            return *this;
        }

        void flush() override {
            m_File.flush();
        }

    private:
        std::fstream m_File;
    };
}  // namespace TK
