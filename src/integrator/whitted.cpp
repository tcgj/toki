#include "whitted.hpp"

#include "core/scene.hpp"
#include "core/scattering.hpp"
#include "core/light.hpp"
#include "core/interaction.hpp"
#include "core/spectrum.hpp"
#include "core/sampler.hpp"
#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum WhittedIntegrator::computeLi(const Scene &scene, const Ray &r,
                                            Sampler &sampler, tkUInt depth) const {
        tkSpectrum li;
        SurfaceInteraction interaction;
        if (!scene.intersect(r, &interaction)) {
            // TODO: get radiance contribution from
            // light source along the ray if any (sky light etc)
            return li;
        }

        // TODO: this is iffy, fix coupling
        Scattering scattering(interaction);
        interaction.computeScattering(&scattering);

        tkVec3f normal = interaction.n;
        tkVec3f wo = interaction.wo;

        // Add emission contribution if interaction with light
        li += interaction.computeLe();

        // Add contribution of lighting on surface point
        for (const auto &light : scene.lights) {
            // Ignore non-delta light source as whitted integrator does not handle
            // if (!light->isDelta())
                // continue;

            tkVec3f wi;
            tkFloat pdf;
            OcclusionChecker occCheck;
            tkVec2f sample;
            sampler.sample2D(&sample, 1, 1);

            tkSpectrum ld = light->sample(interaction, &wi, sample, &pdf, &occCheck);
            if (pdf == 0 || ld.isBlack())
                continue;

            tkSpectrum f = scattering.evaluate(wo, wi);
            if (!f.isBlack() && occCheck.notOccluded(scene))
                li += f * ld * std::abs(dot(wi, normal)) / pdf;
        }

        // Spawn secondary rays from intersection point
        // if depth is under max depth
        if (depth < maxDepth - 1) {
            li += computeReflectedLi(interaction, scene, r, sampler, depth);
            li += computeRefractedLi(interaction, scene, r, sampler, depth);
        }

        return li;
    }
}  // namespace TK
