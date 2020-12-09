#include "formatter.hpp"

namespace TK {
    std::unordered_map<LogLevel, std::string> Formatter::m_Prefix = { { LEVEL_TRACE, "TRACE" },
                                                                      { LEVEL_DEBUG, "DEBUG" },
                                                                      { LEVEL_INFO, "INFO" },
                                                                      { LEVEL_WARNING, "WARNING" },
                                                                      { LEVEL_ERROR, "ERROR" } };

    std::string Formatter::format(LogLevel level, const char* file, int line,
                                  const char* message) const {
        std::ostringstream oss;
        oss << "[ " << m_Prefix[level] << " ] THREAD=" << 1 // get actual thread number in future
            << " at (" << file << ": " << line << "): "
            << message;

        return oss.str();
    }
}  // namespace TK
