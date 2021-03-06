#pragma once

#include "system/toki.hpp"
#include "core/random.hpp"
#include "geometryutil.hpp"

namespace TK {
    /* ----- MIS Heuristics ----- */
    inline tkFloat balanceHeuristic(int nf, tkFloat pdff, int ng,
                                    tkFloat pdfg) {
        return nf * pdff / (nf * pdff + ng * pdfg);
    }

    inline tkFloat powerHeuristic(int nf, tkFloat pdff, int ng,
                                  tkFloat pdfg) {
        // Hard-coded exponent of 2, as empirically determined by Eric Veach
        tkFloat qf = nf * pdff;
        tkFloat qg = ng * pdfg;
        return qf * qf / (qf * qf + qg * qg);
    }

    /* ----- Disk Sampling ----- */
    inline Vec2f rejectionDiskSample() {
        Vec2f v;
        do {
            v.x = 2 * Random::nextFloat() - 1;
            v.y = 2 * Random::nextFloat() - 1;
        } while (v.squaredMagnitude() > 1);
        return v;
    }

    inline Vec2f concentricDiskSample(tkFloat u, tkFloat v) {
        tkFloat a = 2 * u - 1;
        tkFloat b = 2 * v - 1;
        if (a == 0 && b == 0)
            return Vec2f::zero;

        tkFloat phi;
        tkFloat r;
        if (a * a > b * b) {
            r = a;
            phi = TK_PIOVER4 * b / a;
        } else {
            r = b;
            phi = TK_PIOVER2 - TK_PIOVER4 * a / b;
        }

        return Vec2f(r * std::cos(phi), r * std::sin(phi));
    }

    inline Vec2f uniformDiskSample(tkFloat u, tkFloat v) {
        tkFloat r = std::sqrt(u);
        tkFloat theta = 2 * TK_PI * v;
        return Vec2f(r * std::cos(theta), r * std::sin(theta));
    }

    /* ----- Hemisphere Sampling ----- */
    inline Vec3f cosineHemisphereSample(tkFloat u, tkFloat v) {
        Vec2f d = concentricDiskSample(u, v);
        tkFloat z = std::sqrt(std::max((tkFloat)0, 1 - d.x * d.x - d.y * d.y));
        return Vec3f(d, z);
    }

    inline Vec3f uniformHemisphereSample(tkFloat u, tkFloat v) {
        tkFloat theta = std::acos(u);
        tkFloat phi = 2 * TK_PI * v;
        return polarToVec3(theta, phi);
    }

    /* ----- Sphere Sampling ----- */
    inline Vec3f uniformSphereSample(tkFloat u, tkFloat v) {
        tkFloat theta = std::acos(1 - 2 * u);
        tkFloat phi = 2 * TK_PI * v;
        return polarToVec3(theta, phi);
    }

    /* ----- Cone Sampling ----- */
    // cosMaxTheta - cosine from center to edge of the cone
    inline Vec3f uniformConeSample(tkFloat u, tkFloat v, tkFloat cosMaxTheta) {
        tkFloat cosTheta = 1 + (cosMaxTheta - 1) * u;
        tkFloat sinTheta = std::sqrt(1 - cosTheta * cosTheta);
        tkFloat phi = 2 * TK_PI * v;
        return polarToVec3(sinTheta, cosTheta, phi);
    }

    inline tkFloat uniformConePdf(tkFloat cosMaxTheta) {
        return 1 / (2 * TK_PI * (1 - cosMaxTheta));
    }

    /* ----- Triangle Sampling ----- */
    inline Vec2f uniformTriangleSample(tkFloat u, tkFloat v) {
        // Uses the Eric Heitz Square-to-Triangle Mapping
        Vec2f ret(u, v);
        if (v > u) {
            ret.x *= 0.5f;
            ret.y -= ret.x;
        } else {
            ret.y *= 0.5f;
            ret.x -= ret.y;
        }
        return ret;
    }

    /* ----- Distributions ----- */
    class Distribution {
    public:
        Distribution(const std::vector<tkFloat> &f)
            : func(f), cdf(f.size() + 1) {
            int n = f.size();
            if (n == 0)
                LOG(LEVEL_WARNING, "Distribution [ EMPTY DISTRIBUTION SUPPLIED ]");
            cdf[0] = 0;
            for (int i = 1; i < n + 1; ++i) {
                cdf[i] = cdf[i - 1] + f[i - 1] / n;
            }

            integral = cdf[n];
            if (integral > 0) {
                for (int i = 1; i < n + 1; ++i) {
                    cdf[i] /= integral;
                }
            } else {
                for (int i = 1; i < n + 1; ++i) {
                    cdf[i] = (tkFloat)i / n;
                }
            }
        }

        tkFloat sampleContinuous(tkFloat u, tkFloat *pdf = nullptr) const {
            int index = getInterval(cdf.size(), [&](int i) { return cdf[i] <= u; });
            tkFloat offset = u - cdf[index];
            int diff = (cdf[index + 1] - cdf[index]);
            if (diff > 0)
                offset /= diff;
            if (pdf != nullptr)
                *pdf = func[index] / integral;

            return (index + offset) / func.size();
        }

        tkFloat sampleDiscrete(tkFloat u, tkFloat *pdf = nullptr, tkFloat *uRemapped = nullptr) const {
            int index = getInterval(cdf.size(), [&](int i) { return cdf[i] <= u; });
            if (pdf != nullptr)
                *pdf = func[index] / (integral * func.size());
            if (uRemapped != nullptr)
                *uRemapped = (u - cdf[index]) / (cdf[index + 1] - cdf[index]);

            return index;
        }

        std::vector<tkFloat> func;
        std::vector<tkFloat> cdf;
        tkFloat integral;
    };
} // namespace TK
