#include "logger.hpp"

namespace TK {
    Logger::Logger(LogLevel level) : m_Level(level), m_FatalLevel(LEVEL_ERROR) {}

    void Logger::log(LogLevel level, const char* file, int line, const char* format, ...) {
        std::unique_lock<std::mutex> lock(m_Mutex);
        if (level < m_Level)
            return;
        lock.unlock();

        std::va_list vargs;
        char buf[512];
        va_start(vargs, format);
        int len = vsnprintf(buf, sizeof(buf), format, vargs);
        va_end(vargs);

        char* out = buf;
        // update buffer and try again
        if (len + 1 > sizeof(buf)) {
            out = new char[len + 1];
            va_start(vargs, format);
            vsnprintf(out, len + 1, format, vargs);
            va_end(vargs);
        }

        // TODO: Add additional log formatting
        std::string message;
        if (m_Formatter != nullptr)
            message = m_Formatter->format(level, file, line, out);
        else
            message = out;

        if (out != buf)
            delete[] out;

        lock.lock();
        for (auto& a : m_Appenders) {
            a->append(message);
        }

        if (level >= m_FatalLevel)
            throw std::runtime_error("Fatal error occurred.");
    }

    void Logger::setLogLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Level = level;
    }

    LogLevel Logger::getLogLevel() const {
        return m_Level;
    }

    void Logger::setFatalLevel(LogLevel level) {
        m_FatalLevel = level;
    }

    LogLevel Logger::getFatalLevel() const {
        return m_FatalLevel;
    }

    void Logger::setFormatter(std::shared_ptr<Formatter> formatter) {
        m_Formatter = std::move(formatter);
    }

    int Logger::getAppenderCount() const {
        return m_Appenders.size();
    }

    const Appender* Logger::getAppenderAt(int index) const {
        return m_Appenders[index].get();
    }

    void Logger::addAppender(std::shared_ptr<Appender> appender) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (appender == nullptr)
            return;

        m_Appenders.push_back(std::move(appender));
    }

    void Logger::removeAppender(std::shared_ptr<Appender> appender) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        std::remove(m_Appenders.begin(), m_Appenders.end(), appender);
    }

    void Logger::clearAppenders() {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Appenders.clear();
    }

}  // namespace TK
