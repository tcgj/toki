#pragma once

#include "toki.hpp"

namespace TK {
    struct Property;

    class Properties {
    public:
        struct Data;
        enum PropertyType {
            PROP_BOOL,
            PROP_STRING,
            PROP_INT,
            PROP_FLOAT,
            PROP_VECTOR_F,
            PROP_VECTOR_I,
            PROP_POINT_F,
            PROP_POINT_I,
            PROP_SPECTRUM,
            PROP_TRANSFORM,
            PROP_DATA
        };

        Properties(const std::string& id = "unnamed");

        void setId(const std::string& id) {
            m_Id = id;
        }
        std::string getId() const {
            return m_Id;
        }

        bool hasProperty(const std::string& name) const;

        bool deleteProperty(const std::string& name);

        PropertyType getType(const std::string& name) const;

        std::string getStringValue(const std::string& name) const;

        std::string toString() const;

    #define DECL_PROPERTY(type, alias)                                                            \
        void set##alias(const std::string& name, const type& value, bool warnDuplicates = true); \
                                                                                                \
        type get##alias(const std::string& name) const;                                          \
                                                                                                \
        type get##alias(const std::string& name, const type& defaultValue) const;

        DECL_PROPERTY(bool, Bool)
        DECL_PROPERTY(std::string, String)
        DECL_PROPERTY(int, Int)
        DECL_PROPERTY(int64_t, Long)
        DECL_PROPERTY(tkFloat, Float)
        DECL_PROPERTY(Vec3f, FloatVector)
        DECL_PROPERTY(Vec3i, IntVector)
        DECL_PROPERTY(Point3f, FloatPoint)
        DECL_PROPERTY(Point3i, IntPoint)
        DECL_PROPERTY(tkSpectrum, Spectrum)
        DECL_PROPERTY(Transform, Transform)
        DECL_PROPERTY(Properties::Data, Data)

    private:
        std::string m_Id;
        std::unordered_map<std::string, Property> m_Props;
    };
} // namespace TK
