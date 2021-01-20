#include "primitive.hpp"

#include "core/aabb.hpp"
#include "core/ray.hpp"
#include "core/shape.hpp"
#include "material/diffuse.hpp"

namespace TK {
    Primitive::Primitive(std::shared_ptr<Shape> shape, std::shared_ptr<Material> material,
                         std::shared_ptr<Light> light)
        : m_Shape(std::move(shape)), m_Material(std::move(material)), m_Light(std::move(light)) {
        if (m_Material == nullptr)
            m_Material = std::make_shared<Diffuse>(0.6);
    }

    AABBf Primitive::worldBoundingBox() const {
        return m_Shape->worldBoundingBox();
    }

    bool Primitive::intersect(const Ray& r, SurfaceInteraction& out_its) const {
        tkFloat tHit;
        if (!m_Shape->intersect(r, tHit, out_its))
            return false;

        r.tMax = tHit;
        out_its.material = m_Material;
        out_its.light = m_Light;
        return true;
    }

    bool Primitive::hasIntersect(const Ray& r) const {
        return m_Shape->hasIntersect(r);
    }
}  // namespace TK
