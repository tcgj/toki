#include "whitted.hpp"

#include "core/scene.hpp"
#include "core/scattering.hpp"
#include "core/light.hpp"
#include "interaction/surfaceinteraction.hpp"
#include "region/primitive.hpp"
#include "spectrum/rgbspectrum.hpp"
#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum WhittedIntegrator::computeLi(const Scene &scene, const Ray &r,
                                            Sampler &sampler, tkUInt depth) const {
        tkSpectrum li;
        SurfaceInteraction interaction;
        if (!scene.intersect(r, &interaction)) {
            // TODO: get radiance contribution from
            // light source along the ray if any
            return li;
        }

        Scattering scattering(interaction);
        interaction.computeScattering(&scattering);

        tkVec3f normal = interaction.n;
        tkVec3f wo = interaction.wo;

        // TODO: account for emitted radiance from interaction

        // Add contribution of lighting on surface point
        for (const auto &light : scene.lights) {
            tkVec3f wi;
            tkFloat pdf;
            // TODO: Also account for occlusion here

            tkSpectrum ld = light->sample(interaction, &wi, &pdf);
            if (pdf == 0 || ld.isBlack())
                continue;

            tkSpectrum f = scattering(wo, wi);
            if (!f.isBlack())
                li += f * ld * std::abs(dot(wi, normal)) / pdf;
        }

        // Spawn secondary rays from intersection point
        // if depth is under max depth
        if (depth < maxDepth - 1) {
            li += computeReflectionLi(interaction, scene, r, sampler, depth);
            li += computeRefractionLi(interaction, scene, r, sampler, depth);
        }

        return li;
    }
}  // namespace TK
