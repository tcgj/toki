#pragma once

#include "toki.hpp"

namespace TK {
    class Formatter {
    public:
        virtual ~Formatter() {}

        virtual std::string format(LogLevel level, const char* file, int line, const char* message) const;

    private:
        static std::unordered_map<LogLevel, std::string> m_Prefix;
    };
} // namespace TK
