#include "triangle.hpp"

#include "mesh.hpp"
#include "core/aabb.hpp"
#include "core/ray.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    Triangle::Triangle(std::shared_ptr<Mesh> mesh, int64_t triIndex,
                       bool invertNormals)
        : Shape(nullptr, invertNormals), m_Mesh(std::move(mesh)) {
        m_Id = &(m_Mesh->m_IndexBuffer[3 * triIndex]);
    }

    AABBf Triangle::objectBoundingBox() const {
        Point3f& p0 = m_Mesh->m_VertexBuffer[m_Id[0]];
        Point3f& p1 = m_Mesh->m_VertexBuffer[m_Id[1]];
        Point3f& p2 = m_Mesh->m_VertexBuffer[m_Id[2]];

        return bbUnion(AABBf(Point3f(), Point3f(p1 - p0)), Point3f(p2 - p0));
    }

    AABBf Triangle::worldBoundingBox() const {
        Point3f& p0 = m_Mesh->m_VertexBuffer[m_Id[0]];
        Point3f& p1 = m_Mesh->m_VertexBuffer[m_Id[1]];
        Point3f& p2 = m_Mesh->m_VertexBuffer[m_Id[2]];
        return bbUnion(AABBf(p0, p1), p2);
    }

    tkFloat Triangle::surfaceArea() const {
        Point3f& p0 = m_Mesh->m_VertexBuffer[m_Id[0]];
        Point3f& p1 = m_Mesh->m_VertexBuffer[m_Id[1]];
        Point3f& p2 = m_Mesh->m_VertexBuffer[m_Id[2]];

        return cross(p1 - p0, p2 - p0).magnitude() * 0.5;
    }

    // Implementation of Woop et al. Watertight Ray/Triangle Intersection from
    // http://jcgt.org/published/0002/01/05/paper.pdf
    bool Triangle::intersect(const Ray& r, tkFloat& out_tHit, SurfaceInteraction& out_its) const {
        Point3f& p0 = m_Mesh->m_VertexBuffer[m_Id[0]];
        Point3f& p1 = m_Mesh->m_VertexBuffer[m_Id[1]];
        Point3f& p2 = m_Mesh->m_VertexBuffer[m_Id[2]];

        // translate to origin
        Point3f p0t = p0 - Vec3f(r.o);
        Point3f p1t = p1 - Vec3f(r.o);
        Point3f p2t = p2 - Vec3f(r.o);

        // transform max axis to +z
        int z = maxAxis(abs(r.d));
        int x = (z + 1) % 3;
        int y = (x + 1) % 3;
        Vec3f dt = swizzle(r.d, x, y, z);
        p0t = swizzle(p0t, x, y, z);
        p1t = swizzle(p1t, x, y, z);
        p2t = swizzle(p2t, x, y, z);

        // shear transform to align ray to +z
        tkFloat shearX = -dt.x / dt.z;
        tkFloat shearY = -dt.y / dt.z;
        tkFloat shearZ = 1 / dt.z;
        p0t.x = std::fma(shearX, p0t.z, p0t.x);
        p0t.y = std::fma(shearY, p0t.z, p0t.y);
        p1t.x = std::fma(shearX, p1t.z, p1t.x);
        p1t.y = std::fma(shearY, p1t.z, p1t.y);
        p2t.x = std::fma(shearX, p2t.z, p2t.x);
        p2t.y = std::fma(shearY, p2t.z, p2t.y);

        // edge function tests origin to every edge (v_i, v_i+1)
        tkFloat e0 = differenceOfProducts(p1t.x, p2t.y, p2t.x, p1t.y);
        tkFloat e1 = differenceOfProducts(p2t.x, p0t.y, p0t.x, p2t.y);
        tkFloat e2 = differenceOfProducts(p0t.x, p1t.y, p1t.x, p0t.y);

        // fallback to double precision if we get 0 on edges
        if (sizeof(tkFloat) == sizeof(float) && (e0 == 0 || e1 == 0 || e2 == 0)) {
            double temp0 = (double)p1t.x * (double)p2t.y;
            double temp1 = (double)p2t.x * (double)p1t.y;
            e0 = (float)(temp0 - temp1);
            temp0 = (double)p2t.x * (double)p0t.y;
            temp1 = (double)p0t.x * (double)p2t.y;
            e1 = (float)(temp0 - temp1);
            temp0 = (double)p0t.x * (double)p1t.y;
            temp1 = (double)p1t.x * (double)p0t.y;
            e2 = (float)(temp0 - temp1);
        }

        // if signs are different, not intersecting
        if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0))
            return false;

        // get determinant
        tkFloat det = e0 + e1 + e2;
        if (det == 0)
            return false;

        // test scaled t-value for intersection
        p0t.z *= shearZ;
        p1t.z *= shearZ;
        p2t.z *= shearZ;
        tkFloat tScaled = p0t.z * e0 + p1t.z * e1 + p2t.z * e2;
        if (det < 0 && (tScaled >= 0 || tScaled < r.tMax * det))
            return false;
        else if (det > 0 && (tScaled <= 0 || tScaled > r.tMax * det))
            return false;

        // calculate intersection values
        tkFloat invDet = 1 / det;
        tkFloat b0 = e0 * invDet;
        tkFloat b1 = e1 * invDet;
        tkFloat b2 = e2 * invDet;
        tkFloat t = tScaled * invDet;

        // uv mapping
        Vec3f v01 = p1 - p0;
        Vec3f v02 = p2 - p0;
        Vec3f normal = cross(v01, v02).normalized();

        Point2f& uv0 = m_Mesh->m_UvBuffer[m_Id[0]];
        Vec2f duv1;
        Vec2f duv2;
        if (m_Mesh->m_UvBuffer != nullptr) {
            duv1 = m_Mesh->m_UvBuffer[m_Id[1]] - uv0;
            duv2 = m_Mesh->m_UvBuffer[m_Id[2]] - uv0;
        }
        tkFloat uvDet = duv1[0] * duv2[1] - duv1[1] * duv2[0];
        Vec3f tangent;
        Vec3f bitangent;
        if (uvDet == 0)
            coordinateSystem(normal, &tangent, &bitangent);
        else {
            invDet = 1 / uvDet;
            tangent = normalize((duv2[1] * v01 - duv1[1] * v02) * invDet);
            bitangent = normalize((duv1[0] * v02 - duv2[0] * v01) * invDet);
        }

        out_tHit = t;
        Point3f hitP = b0 * p0 + b1 * p1 + b2 * p2;
        // TODO: set shading normal based on whether normal was provided for vertices
        out_its = SurfaceInteraction(hitP, m_InvertNormals ? -normal : normal, tangent, bitangent,
                                          normalize(-r.d), this);
        return true;
    }

    bool Triangle::hasIntersect(const Ray& r) const {
        Point3f& p0 = m_Mesh->m_VertexBuffer[m_Id[0]];
        Point3f& p1 = m_Mesh->m_VertexBuffer[m_Id[1]];
        Point3f& p2 = m_Mesh->m_VertexBuffer[m_Id[2]];

        // translate to origin
        Point3f p0t = p0 - Vec3f(r.o);
        Point3f p1t = p1 - Vec3f(r.o);
        Point3f p2t = p2 - Vec3f(r.o);

        // transform max axis to +z
        int z = maxAxis(abs(r.d));
        int x = (z + 1) % 3;
        int y = (x + 1) % 3;
        Vec3f dt = swizzle(r.d, x, y, z);
        p0t = swizzle(p0t, x, y, z);
        p1t = swizzle(p1t, x, y, z);
        p2t = swizzle(p2t, x, y, z);

        // shear transform to align ray to +z
        tkFloat shearX = -dt.x / dt.z;
        tkFloat shearY = -dt.y / dt.z;
        tkFloat shearZ = 1 / dt.z;
        p0t.x = std::fma(shearX, p0t.z, p0t.x);
        p0t.y = std::fma(shearY, p0t.z, p0t.y);
        p1t.x = std::fma(shearX, p1t.z, p1t.x);
        p1t.y = std::fma(shearY, p1t.z, p1t.y);
        p2t.x = std::fma(shearX, p2t.z, p2t.x);
        p2t.y = std::fma(shearY, p2t.z, p2t.y);

        // edge function tests origin to every edge (v_i, v_i+1)
        tkFloat e0 = differenceOfProducts(p1t.x, p2t.y, p2t.x, p1t.y);
        tkFloat e1 = differenceOfProducts(p2t.x, p0t.y, p0t.x, p2t.y);
        tkFloat e2 = differenceOfProducts(p0t.x, p1t.y, p1t.x, p0t.y);

        // fallback to double precision if we get 0 on edges
        if (sizeof(tkFloat) == sizeof(float) && (e0 == 0 || e1 == 0 || e2 == 0)) {
            double temp0 = (double)p1t.x * (double)p2t.y;
            double temp1 = (double)p2t.x * (double)p1t.y;
            e0 = (float)(temp0 - temp1);
            temp0 = (double)p2t.x * (double)p0t.y;
            temp1 = (double)p0t.x * (double)p2t.y;
            e1 = (float)(temp0 - temp1);
            temp0 = (double)p0t.x * (double)p1t.y;
            temp1 = (double)p1t.x * (double)p0t.y;
            e2 = (float)(temp0 - temp1);
        }

        // if signs are different, not intersecting
        if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0))
            return false;

        // get determinant
        tkFloat det = e0 + e1 + e2;
        if (det == 0)
            return false;

        // test scaled t-value for intersection
        p0t.z *= shearZ;
        p1t.z *= shearZ;
        p2t.z *= shearZ;
        tkFloat tScaled = p0t.z * e0 + p1t.z * e1 + p2t.z * e2;
        if (det < 0 && (tScaled >= 0 || tScaled < r.tMax * det))
            return false;
        else if (det > 0 && (tScaled <= 0 || tScaled > r.tMax * det))
            return false;

        return true;
    }

    SurfaceInteraction Triangle::sample(const Interaction& ref, const Vec2f& u, tkFloat& out_pdf) const {
        SurfaceInteraction ret;

        Point3f& p0 = m_Mesh->m_VertexBuffer[m_Id[0]];
        Point3f& p1 = m_Mesh->m_VertexBuffer[m_Id[1]];
        Point3f& p2 = m_Mesh->m_VertexBuffer[m_Id[2]];
        Vec2f bCoord = uniformTriangleSample(u[0], u[1]);
        tkFloat bCoordZ = (1 - bCoord.x - bCoord.y);

        ret.p = bCoord.x * p0 + bCoord.y * p1 + bCoordZ * p2;
        if (m_Mesh->m_NormalBuffer != nullptr)
            ret.n = bCoord.x * m_Mesh->m_NormalBuffer[m_Id[0]] + bCoord.y * m_Mesh->m_NormalBuffer[m_Id[1]] +
                    bCoordZ * m_Mesh->m_NormalBuffer[m_Id[2]];
        else
            ret.n = normalize(cross(p1 - p0, p2 - p0));
        if (m_InvertNormals)
            ret.n = -ret.n;
        ret.wo = normalize(ref.p - ret.p);

        out_pdf = getPdf(ref, ret);
        return ret;
    }
}  // namespace TK