#pragma once

#include <fstream>

#include "stream.hpp"

namespace TK {
    enum FileStreamMode {
        FSM_IN = std::ios_base::in | std::ios_base::binary,
        FSM_OUT = std::ios_base::out | std::ios_base::binary,
        FSM_IO = FSM_IN | FSM_OUT
    };

    class FileStream : public IOStream {
    public:
        FileStream() = default;
        FileStream(const std::string filename, FileStreamMode fsm)
            : file(std::fstream(filename, fsm)), reading(fsm & FSM_IN) {
            // TODO: Setup a proper check for fail
            tkAssert(!file.is_open());
        }

        bool open(std::string filename, FileStreamMode fsm,
                  bool forceOpen = false) {
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
        FileStream &operator>>(tkFloat &f) override {
            if (!reading)
                switchIO();
            file.read(reinterpret_cast<char *>(&f), sizeof(f));
            return *this;
        }

        FileStream &operator>>(tkInt &i) override {
            if (!reading)
                switchIO();
            file.read(reinterpret_cast<char *>(&i), sizeof(i));
            return *this;
        }

        FileStream &operator>>(tkUInt &u) override {
            if (!reading)
                switchIO();
            file.read(reinterpret_cast<char *>(&u), sizeof(u));
            return *this;
        }

        FileStream &operator>>(std::string &s) override {
            if (!reading)
                switchIO();
            s = "";
            tkChar c;
            do {
                file.read(&c, sizeof(c));
                if (c == 0)
                    break;
                s += c;
            } while (file.good());
            return *this;
        }

        FileStream &operator>>(bool &b) override {
            if (!reading)
                switchIO();
            file.read(reinterpret_cast<char *>(&b), sizeof(b));
            return *this;
        }

        FileStream &read(tkChar *data, tkUInt size) override {
            if (!reading)
                switchIO();
            file.read(data, size);
            return *this;
        }

        // OStream operators
        FileStream &operator<<(tkFloat f) override {
            if (reading)
                switchIO();
            file.write(reinterpret_cast<const char *>(&f), sizeof(f));
            return *this;
        }

        FileStream &operator<<(tkInt i) override {
            if (reading)
                switchIO();
            file.write(reinterpret_cast<const char *>(&i), sizeof(i));
            return *this;
        }

        FileStream &operator<<(tkUInt u) override {
            if (reading)
                switchIO();
            file.write(reinterpret_cast<const char *>(&u), sizeof(u));
            return *this;
        }

        FileStream &operator<<(const std::string &s) override {
            if (reading)
                switchIO();
            file.write(s.c_str(), s.size() + 1);
            return *this;
        }

        FileStream &operator<<(bool b) override {
            if (reading)
                switchIO();
            file.write(reinterpret_cast<const char *>(&b), sizeof(b));
            return *this;
        }

        FileStream &write(const tkChar *data, tkUInt size) override {
            if (reading)
                switchIO();
            file.write(data, size);
            return *this;
        }

        void flush() override {
            file.flush();
        }

    private:
        void switchIO() {
            if (reading) {
                readPos = file.tellg();
                file.seekp(0, std::ios_base::end);
            } else {
                file.seekg(readPos);
            }
            reading = !reading;
        }

        bool reading;
        std::streampos readPos;
        std::fstream file;
    };
} // namespace TK
