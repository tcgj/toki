#pragma once

#include "properties.hpp"

namespace TK {
    class SerializableObject {
    public:
        const Properties& getProperties() const {
            return m_Props;
        }

    protected:
        SerializableObject(const Properties& props) : m_Props(props) {}

        Properties m_Props;
    };
}  // namespace TK
