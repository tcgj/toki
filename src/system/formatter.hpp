#pragma once

#include "toki.hpp"

namespace TK {
    enum LogLevel {
        LEVEL_TRACE,
        LEVEL_DEBUG,
        LEVEL_INFO,
        LEVEL_WARNING,
        LEVEL_ERROR
    };

    class Formatter {
    public:
        virtual ~Formatter() {}
        virtual std::string format(LogLevel level, const char* file, int line, const char* message) const;

    private:
        static std::unordered_map<LogLevel, std::string> m_Prefix;
    };
} // namespace TK
