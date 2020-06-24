#pragma once

#include "core/shape.hpp"
#include "curveutil.hpp"

namespace TK {
    class Curve : public Shape {
    public:
        Curve(const std::shared_ptr<CurveCommon> &common, tkFloat uMin,
              tkFloat uMax, const Transform *objectToWorld)
            : Shape(objectToWorld, false), common(common), uMin(uMin), uMax(uMax) {}

        tkAABBf objectBoundingBox() const override;
        bool intersect(const Ray &r, tkFloat *tHit,
                       SurfaceInteraction *interaction) const override;
        bool hasIntersect(const Ray &r) const override;

        bool recursiveIntersect(const Ray &r, tkFloat *tHit,
                                SurfaceInteraction *interaction,
                                const tkPoint3f cp[4],
                                const Transform &rayToObject, tkAABBf &rayBB,
                                tkFloat rayLength, tkFloat u0, tkFloat u1,
                                tkUInt depth) const;

        tkAABBf computeBoundingBox(const tkPoint3f cp[4], tkFloat u0,
                                   tkFloat u1) const;
        void computeSegmentControlPoints(tkPoint3f cp[4]) const;

    private:
        std::shared_ptr<CurveCommon> common;
        tkFloat uMin, uMax;
    };
}  // namespace TK