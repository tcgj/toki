#include "properties.hpp"

#include <variant>

#include "core/transform.hpp"
#include "core/spectrum.hpp"

namespace TK {
    using Element = std::variant<bool, std::string, int64_t, tkFloat, Vec3f, Vec3i, Point3f, Point3i,
                                 tkSpectrum, Transform, Properties::Data>;
    struct Property {
        Element data;
        Properties::PropertyType type;
    };

    namespace {
        class StringifyVisitor {
        public:
            StringifyVisitor(std::ostringstream& oss) : oss(oss) {}

            void operator()(const bool& v) const { oss << (v ? "true" : "false"); }
            void operator()(const std::string& v) const { oss << v; }
            void operator()(const int64_t& v) const { oss << v; }
            void operator()(const tkFloat& v) const { oss << v; }
            void operator()(const Vec3f& v) const { oss << v.toString(); }
            void operator()(const Vec3i& v) const { oss << v.toString(); }
            void operator()(const Point3f& v) const { oss << v.toString(); }
            void operator()(const Point3i& v) const { oss << v.toString(); }
            void operator()(const tkSpectrum& v) const { oss << v.toString(); }
            void operator()(const Transform& v) const { oss << v.toString(); }
            void operator()(const Properties::Data& v) const { oss << v.ptr << " (size: " << v.size << ")"; }

        private:
            std::ostringstream& oss;
        };
    } // namespace __

    Properties::Properties(const std::string& id) : m_Id(id) {}

    bool Properties::hasProperty(const std::string& name) const {
        return m_Props.count(name) > 0;
    }

    bool Properties::deleteProperty(const std::string& name) {
        if (!hasProperty(name))
            return false;
        m_Props.erase(name);
        return true;
    }

    Properties::PropertyType Properties::getType(const std::string& name) const {
        auto result = m_Props.find(name);
        // if (result == m_Props.end())
            // Log failure and exit
        return result->second.type;
    }

    std::string Properties::getStringValue(const std::string& name) const {
        auto result = m_Props.find(name);
        // if (result == m_Props.end())
        // Log failure and exit
        std::ostringstream oss;
        std::visit(StringifyVisitor(oss), result->second.data);
        return oss.str();
    }

    std::string Properties::toString() const {
        std::ostringstream oss;
        StringifyVisitor sVisitor(oss);
        oss << "Properties (Id=\"" << m_Id << "\") [\n";

        for (const auto& prop : m_Props) {
            oss << "    \"" << prop.first << "\":  ";
            std::visit(sVisitor, prop.second.data);
            oss << "\n";
        }
        oss << "]\n";
        return oss.str();
    }

#define INIT_PROPERTY(type, storeType, alias)                                                             \
    void Properties::set##alias(const std::string& name, const type& value, bool warnDuplicates) { \
        if (!hasProperty(name) && warnDuplicates)                                                         \
            printf("Does not exist");                                                                     \
        m_Props[name].data = (storeType)value;                                                            \
    }                                                                                                     \
                                                                                                          \
    type Properties::get##alias(const std::string& name) const {                                          \
        auto result = m_Props.find(name);                                                                 \
        /* if (result == m_Props.end()) */                                                                \
        /* Log failure and exit */                                                                        \
        auto val = std::get_if<storeType>(&result->second.data);                                          \
        /* if (!val) */                                                                                   \
        /* Log failure */                                                                                 \
        return (type)*val;                                                                                \
    }                                                                                                     \
                                                                                                          \
    type Properties::get##alias(const std::string& name, const type& defaultValue) const {                \
        auto result = m_Props.find(name);                                                                 \
        if (result == m_Props.end())                                                                      \
            return defaultValue;                                                                          \
        auto val = std::get_if<storeType>(&result->second.data);                                          \
        /* if (!val) */                                                                                   \
        /* Log failure */                                                                                 \
        return (type)*val;                                                                                \
    }

    INIT_PROPERTY(bool, bool, Bool)
    INIT_PROPERTY(std::string, std::string, String)
    INIT_PROPERTY(int, int64_t, Int)
    INIT_PROPERTY(int64_t, int64_t, Long)
    INIT_PROPERTY(tkFloat, tkFloat, Float)
    INIT_PROPERTY(Vec3f, Vec3f, FloatVector)
    INIT_PROPERTY(Vec3i, Vec3i, IntVector)
    INIT_PROPERTY(Point3f, Point3f, FloatPoint)
    INIT_PROPERTY(Point3i, Point3i, IntPoint)
    INIT_PROPERTY(tkSpectrum, tkSpectrum, Spectrum)
    INIT_PROPERTY(Transform, Transform, Transform)
    INIT_PROPERTY(Properties::Data, Properties::Data, Data)

}  // namespace TK