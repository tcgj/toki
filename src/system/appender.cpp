#include "appender.hpp"

namespace TK {
    StreamAppender::StreamAppender(std::shared_ptr<std::ostream> s) : m_Stream(std::move(s)) {}

    void StreamAppender::append(const std::string& message) {
        (*m_Stream) << message << "\n";
    }

    FileAppender::FileAppender(const std::string& fileName) : m_FileName(fileName) {
        m_Stream = std::make_shared<std::fstream>(fileName, std::fstream::out | std::fstream::trunc);
    }

    std::string FileAppender::getFileName() const {
        return m_FileName;
    }
}  // namespace TK
