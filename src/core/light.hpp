#pragma once

#include "system/toki.hpp"
#include "transform.hpp"
#include "scene.hpp"
#include "interaction.hpp"

namespace TK {
    class OcclusionChecker {
    public:
        OcclusionChecker() = default;
        OcclusionChecker(const Interaction &p0, const Interaction &p1)
            : p0(p0), p1(p1) {}

        bool notOccluded(const Scene &scene) const {
            return !scene.hasIntersect(p0.spawnRayTo(p1));
        }

    private:
        Interaction p0, p1;
    };

    class Light {
    public:
        Light(const Transform &lightToWorld) : lightToWorld(lightToWorld) {}
        virtual ~Light() = default;

        virtual void preprocess(const Scene &scene) {}
        virtual bool isDelta() const {
            // Whether the light source is described by a delta distribution,
            // positional wise or directional-wise
            return true;
        }
        virtual tkSpectrum power() const = 0;
        virtual tkSpectrum computeLe(const SurfaceInteraction &interaction, const tkVec3f &w) const {
            return 0;
        };
        virtual tkSpectrum sample(const Interaction &ref, tkVec3f *wi,
                                  tkFloat *pdf, OcclusionChecker *occCheck) const = 0;
        virtual tkFloat getPdf(const Interaction &ref, const tkVec3f &wi) const {
            return 0;
        }

    protected:
        Transform lightToWorld;
    };
} // namespace TK
