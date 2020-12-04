#pragma once

#include "toki.hpp"

namespace TK {
    struct Property;

    class Properties {
    public:
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

        struct Data {
            uint8_t* ptr;
            size_t size;
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

        void setBool(const std::string& name, const bool& value, bool warnDuplicates = true);
        bool getBool(const std::string& name) const;
        bool getBool(const std::string& name, const bool& defaultValue) const;

        void setString(const std::string& name, const std::string& value, bool warnDuplicates = true);
        std::string getString(const std::string& name) const;
        std::string getString(const std::string& name, const std::string& defaultValue) const;

        void setInt(const std::string& name, const int& value, bool warnDuplicates = true);
        int getInt(const std::string& name) const;
        int getInt(const std::string& name, const int& defaultValue) const;

        void setLong(const std::string& name, const int64_t& value, bool warnDuplicates = true);
        int64_t getLong(const std::string& name) const;
        int64_t getLong(const std::string& name, const int64_t& defaultValue) const;

        void setFloat(const std::string& name, const tkFloat& value, bool warnDuplicates = true);
        tkFloat getFloat(const std::string& name) const;
        tkFloat getFloat(const std::string& name, const tkFloat& defaultValue) const;

        void setFloatVector(const std::string& name, const Vec3f& value, bool warnDuplicates = true);
        Vec3f getFloatVector(const std::string& name) const;
        Vec3f getFloatVector(const std::string& name, const Vec3f& defaultValue) const;

        void setIntVector(const std::string& name, const Vec3i& value, bool warnDuplicates = true);
        Vec3i getIntVector(const std::string& name) const;
        Vec3i getIntVector(const std::string& name, const Vec3i& defaultValue) const;

        void setFloatPoint(const std::string& name, const Point3f& value, bool warnDuplicates = true);
        Point3f getFloatPoint(const std::string& name) const;
        Point3f getFloatPoint(const std::string& name, const Point3f& defaultValue) const;

        void setIntPoint(const std::string& name, const Point3i& value, bool warnDuplicates = true);
        Point3i getIntPoint(const std::string& name) const;
        Point3i getIntPoint(const std::string& name, const Point3i& defaultValue) const;

        void setSpectrum(const std::string& name, const tkSpectrum& value, bool warnDuplicates = true);
        tkSpectrum getSpectrum(const std::string& name) const;
        tkSpectrum getSpectrum(const std::string& name, const tkSpectrum& defaultValue) const;

        void setTransform(const std::string& name, const Transform& value, bool warnDuplicates = true);
        Transform getTransform(const std::string& name) const;
        Transform getTransform(const std::string& name, const Transform& defaultValue) const;

        void setData(const std::string& name, const Data& value, bool warnDuplicates = true);
        Data getData(const std::string& name) const;
        Data getData(const std::string& name, const Data& defaultValue) const;

    private:
        std::string m_Id;
        std::unordered_map<std::string, Property> m_Props;
    };
} // namespace TK
