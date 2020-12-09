#pragma once

#include "toki.hpp"

namespace TK {
    class Appender {
    public:
        virtual ~Appender() = default;

        virtual void append(const std::string& message) = 0;
    };

    class StreamAppender : public Appender {
    public:
        StreamAppender(std::shared_ptr<std::ostream> s);

        void append(const std::string& message) override;

    protected:
        StreamAppender() = default;

        std::shared_ptr<std::ostream> m_Stream;
    };

    class FileAppender : public StreamAppender {
    public:
        FileAppender(const std::string& fileName);

        std::string getFileName() const;

    private:
        std::string m_FileName;
    };
} // namespace TK
