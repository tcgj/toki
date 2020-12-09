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

        FileStream(const std::string filename, FileStreamMode fsm) : file(std::fstream(filename, fsm)) {
            // TODO: Setup a proper check for fail
        }

        bool open(std::string filename, FileStreamMode fsm, bool forceOpen = false) {
            if (file.is_open()) {
                if (forceOpen)
                    file.close();
                else
                    return false;
            }
            file.open(filename, fsm);
            return file.is_open();
        }

        bool close() {
            if (!file.is_open())
                return false;
            file.close();
            return true;
        }

        // IStream operators
        FileStream& operator>>(tkFloat& f) override {
            file.read(reinterpret_cast<char*>(&f), sizeof(f));
            return *this;
        }

        FileStream& operator>>(int& i) override {
            file.read(reinterpret_cast<char*>(&i), sizeof(i));
            return *this;
        }

        FileStream& operator>>(unsigned int& u) override {
            file.read(reinterpret_cast<char*>(&u), sizeof(u));
            return *this;
        }

        FileStream& operator>>(std::string& s) override {
            s = "";
            char c;
            do {
                file.read(&c, sizeof(c));
                if (c == 0)
                    break;
                s += c;
            } while (file.good());
            return *this;
        }

        FileStream& operator>>(bool& b) override {
            file.read(reinterpret_cast<char*>(&b), sizeof(b));
            return *this;
        }

        FileStream& read(char* data, unsigned int size) override {
            file.read(data, size);
            return *this;
        }

        // OStream operators
        FileStream& operator<<(tkFloat f) override {
            file.write(reinterpret_cast<const char*>(&f), sizeof(f));
            return *this;
        }

        FileStream& operator<<(int i) override {
            file.write(reinterpret_cast<const char*>(&i), sizeof(i));
            return *this;
        }

        FileStream& operator<<(unsigned int u) override {
            file.write(reinterpret_cast<const char*>(&u), sizeof(u));
            return *this;
        }

        FileStream& operator<<(const std::string& s) override {
            file.write(s.c_str(), s.size() + 1);
            return *this;
        }

        FileStream& operator<<(bool b) override {
            file.write(reinterpret_cast<const char*>(&b), sizeof(b));
            return *this;
        }

        FileStream& write(const char* data, unsigned int size) override {
            file.write(data, size);
            return *this;
        }

        void flush() override {
            file.flush();
        }

    private:
        std::fstream file;
    };
}  // namespace TK
