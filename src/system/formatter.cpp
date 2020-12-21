#include "formatter.hpp"

#include "thread.hpp"

namespace TK {
    std::unordered_map<LogLevel, std::string> Formatter::m_Prefix = { { LEVEL_TRACE, "TRACE" },
                                                                      { LEVEL_DEBUG, "DEBUG" },
                                                                      { LEVEL_INFO, "INFO" },
                                                                      { LEVEL_WARNING, "WARNING" },
                                                                      { LEVEL_ERROR, "ERROR" } };

    std::string Formatter::format(LogLevel level, const char* file, int line,
                                  const char* message) const {
        std::ostringstream oss;
        ThreadPool* pool = g_Context.threadpool();
        int id = pool != nullptr ? pool->getThreadId() : 0;
        oss << "[ " << m_Prefix[level] << " ] THREAD=" << id
            << " at (" << file << ": " << line << "): "
            << message;

        return oss.str();
    }
}  // namespace TK
