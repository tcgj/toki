#include "curve.hpp"

#include "geometry/ray.hpp"

namespace TK {
    inline tkAABBf Curve::objectBoundingBox() const {
        tkPoint3f cp[4];
        computeSegmentControlPoints(cp);

        return computeBoundingBox(cp, uMin, uMax);
    }

    inline tkAABBf Curve::computeBoundingBox(const tkPoint3f cp[4], tkFloat u0, tkFloat u1) const {
        tkAABBf bb = bbUnion(tkAABBf(cp[0], cp[1]), tkAABBf(cp[2], cp[3]));
        tkFloat maxWidth = std::max(lerp(u0, common->width[0], common->width[1]),
                                    lerp(u1, common->width[0], common->width[1]));
        return expand(bb, maxWidth * 0.5);
    }

    bool Curve::intersect(const Ray &r, tkFloat *tHit,
                   SurfaceInteraction *interaction) const {
        Ray oRay = (inverse(*objectToWorld))(r);
        tkPoint3f cpObj[4];
        computeSegmentControlPoints(cpObj);

        tkVec3f dx = cross(oRay.d, cpObj[3] - cpObj[0]);
        if (dx.squaredMagnitude() == 0) {
            tkVec3f dy;
            coordinateSystem(oRay.d, &dx, &dy);
        }

        Transform objectToRay = lookAt(oRay.o, oRay.o + oRay.d, dx);
        tkPoint3f cp[4] = {objectToRay(cpObj[0]), objectToRay(cpObj[1]),
                           objectToRay(cpObj[2]), objectToRay(cpObj[3])};

        // ensure bounding box overlap
        // calculate bounding box of curve segment
        tkAABBf curveBB = computeBoundingBox(cp, uMin, uMax);

        // calculate bounding box of ray
        tkFloat rayLen = oRay.d.magnitude();
        tkFloat zMax = rayLen * oRay.tMax;
        tkAABBf rayBB(tkPoint3f::zero, tkPoint3f(0, 0, zMax));

        // ensure overlap
        if (!overlap(curveBB, rayBB))
            return false;

        // Maximum depth calculation based on prbt
        tkFloat l0 = 0;
        for (tkUInt i = 0; i < 2; ++i) {
            l0 = std::max(l0,
                std::max(std::max(std::abs(cp[i].x - 2 * cp[i + 1].x + cp[i + 2].x),
                                  std::abs(cp[i].y - 2 * cp[i + 1].y + cp[i + 2].y)),
                         std::abs(cp[i].z - 2 * cp[i + 1].z + cp[i + 2].z)));
        }
        tkFloat eps = std::max(common->width[0], common->width[1]) * 0.05;
        tkFloat fr0 = std::log(1.41421356237 * 12.0 * l0 / (8.0 * eps)) * 0.7213475108;
        tkInt r0 = (tkInt)std::round(fr0);
        tkUInt maxDepth = clamp(r0, 0, 10);

        return recursiveIntersect(oRay, tHit, interaction, cp,
                                  inverse(objectToRay), rayBB, rayLen, uMin,
                                  uMax, maxDepth);
    }

    bool Curve::hasIntersect(const Ray &r) const {
        return intersect(r, nullptr, nullptr);
    }

    bool Curve::recursiveIntersect(const Ray &r, tkFloat *tHit,
                                   SurfaceInteraction *interaction,
                                   const tkPoint3f cp[4],
                                   const Transform &rayToObject, tkAABBf &rayBB,
                                   tkFloat rayLength, tkFloat u0, tkFloat u1,
                                   tkUInt depth) const {
        if (depth <= 0) {
            // Base case, bounding box overlaps

            // edge function used to test if a point is on the left or right side of an edge
            // edge > 0 implies point is on the "inside" of the edge
            tkFloat edge = (cp[1].y - cp[0].y) * -cp[0].y + cp[0].x * (cp[0].x - cp[1].x);
            if (edge < 0)
                return false;

            edge = (cp[2].y - cp[3].y) * -cp[3].y + cp[3].x * (cp[3].x - cp[2].x);
            if (edge < 0)
                return false;

            tkVec2f segDir = tkPoint2f(cp[3]) - tkPoint2f(cp[0]);
            tkFloat denom = segDir.squaredMagnitude();
            if (denom == 0)
                return false;
            // d is the distance of cp0 from the intersection point, (0,0), along the line cp3-cp0
            // w is the ratio of the distance d to the line cp3-cp0
            tkFloat w = dot(-tkVec2f(cp[0]), segDir) / denom;

            tkFloat u = clamp(lerp(u0, u1, w), u0, u1);
            tkFloat width = lerp(common->width[0], common->width[1], w);
            tkVec3f normal;

            // ribbon's width and normal have to be handled differently
            if (common->type == CurveType::RIBBON) {
                // slerp normal based on the difference in their angles
                tkFloat sin0 = std::sin((1 - u) * common->normalAngle) * common->invSinNormalAngle;
                tkFloat sin1 = std::sin(u * common->normalAngle) * common->invSinNormalAngle;
                normal = sin0 * common->normal[0] + sin1 * common->normal[1];

                // update width based on rotation
                width *= std::abs(dot(normal, r.d)) / rayLength;
            }

            // Ensure intersection point is within curve's width
            tkVec3f dp;
            tkPoint3f p = bezierEval(cp, clamp(w, 0.0f, 1.0f), &dp);
            tkFloat pSqrLen2d = p.x * p.x + p.y * p.y;
            tkFloat widthSqrOver4 = width * width * 0.25;
            if (pSqrLen2d > widthSqrOver4)
                return false;
            if (p.z < 0 || p.z > rayLength * r.tMax)
                return false;

            tkFloat pLen2d = std::sqrt(pSqrLen2d);
            //  = dp.x * -p.y + p.x * dp.y;
            // kFloat v = edge > 0 ? 2 * pLen2d / width : -2 * pLen2d / width;

            // calculate normal at intersection
            if (common->type == CurveType::CYLINDER) {
                // transform to cross section
                tkVec3f dup = tkVec3f(0, 1, 0); // FIXIT: zero if vectors are parallel
                tkVec3f dright = normalize(cross(dup, dp));
                if (dot(dright, r.d) > 0)
                    dright = -dright;
                tkPoint3f pCentre(p);
                p += pLen2d * dup + std::sqrt(widthSqrOver4 - pSqrLen2d) * dright;
                normal = p - pCentre;
            } else if (common->type == CurveType::FLAT) {
                normal = -r.d;
            }

            if (tHit) { // not a shadow ray
                tkFloat tempT = p.z / rayLength;
                if (tempT < TK_EPSILON || tempT > r.tMax)
                    return false;

                *tHit = tempT; // Apparently not accurate for ribbons
                interaction->p = (*objectToWorld)(r(*tHit));
                interaction->n = normalize((*objectToWorld)(normal));
                interaction->wo = normalize((*objectToWorld)(-r.d));
                interaction->shape = this;
            }

            return true;
        } else {
            // Recursive case, split curve
            tkPoint3f cpSplit[7];
            bezierSubdivide(cp, cpSplit);

            bool hit = false;
            tkFloat u[3] = {u0, (u0 + u1) / 2, u1};
            const tkPoint3f *cpPtr = cpSplit;
            for (tkUInt seg = 0; seg < 2; ++seg, cpPtr += 3) {
                tkAABBf curveBB = computeBoundingBox(cpPtr, u[seg], u[seg + 1]);
                if (!overlap(curveBB, rayBB))
                    continue;

                hit |= recursiveIntersect(r, tHit, interaction, cpPtr,
                                          rayToObject, rayBB, rayLength, u[seg],
                                          u[seg + 1], depth - 1);

                if (hit && !tHit)  // tHit is nullptr, this is a shadow ray
                    return true;
            }
            return hit;
        }
    }

    inline void Curve::computeSegmentControlPoints(tkPoint3f cp[4]) const {
        cp[0] = bezierBlossom(common->cp, uMin, uMin, uMin);
        cp[1] = bezierBlossom(common->cp, uMin, uMin, uMax);
        cp[2] = bezierBlossom(common->cp, uMin, uMax, uMax);
        cp[3] = bezierBlossom(common->cp, uMax, uMax, uMax);
    };
} // namespace TK