#pragma once

#include "toki.hpp"
#include "appender.hpp"
#include "formatter.hpp"

namespace TK {

#define LOG(level, format, ...) {\
    /* get logger for current thread*/ \
    /* logger.log(level, __FILE__, __LINE__, format, __VA_ARGS__);*/ \
}

    class Logger {
    public:
        Logger(LogLevel level);

        void log(LogLevel level, const char* file, int line, const char* format, ...);

        void setLogLevel(LogLevel level);

        LogLevel getLogLevel() const;

        void setFatalLevel(LogLevel level);

        LogLevel getFatalLevel() const;

        void setFormatter(std::shared_ptr<Formatter> formatter);

        int getAppenderCount() const;

        const Appender* getAppenderAt(int index) const;

        void addAppender(std::shared_ptr<Appender> appender);

        void removeAppender(std::shared_ptr<Appender> appender);

        void clearAppenders();

    private:
        LogLevel m_Level;
        LogLevel m_FatalLevel;
        std::shared_ptr<Formatter> m_Formatter;
        std::vector<std::shared_ptr<Appender>> m_Appenders;
        std::mutex m_Mutex;
    };
} // namespace TK
