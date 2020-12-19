#include "appender.hpp"

namespace TK {
    StreamAppender::StreamAppender(std::ostream* s) : m_Stream(s) {}

    void StreamAppender::append(const std::string& message) {
        (*m_Stream) << message << "\n";
    }

    FileAppender::FileAppender(const std::string& fileName) : m_FileName(fileName) {
        m_Stream = new std::fstream(fileName, std::fstream::out | std::fstream::trunc);
    }

    FileAppender::~FileAppender() {
        delete m_Stream;
    }

    std::string FileAppender::getFileName() const {
        return m_FileName;
    }
}  // namespace TK
