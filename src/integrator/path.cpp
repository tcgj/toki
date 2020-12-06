#include "path.hpp"

#include "core/scene.hpp"
#include "core/bsdf.hpp"
#include "core/light.hpp"
#include "core/interaction.hpp"
#include "core/spectrum.hpp"
#include "core/sampler.hpp"
#include "util/scatteringutil.hpp"

namespace TK {
    void PathTracingIntegrator::preprocess(const Scene& scene) {
        switch (strategy) {
            case POWER:
                lightDist = lightPowerDistribution(scene);
                break;
            default:
                break;
        }
    }

    tkSpectrum PathTracingIntegrator::Li(const Scene& scene, const Ray& r, Sampler& sampler,
                                         int depth) const {
        tkSpectrum li;
        tkSpectrum throughput(1);
        Ray ray(r);
        SurfaceInteraction interaction;

        for (int bounces = 0;; ++bounces) {
            bool hit = scene.intersect(ray, &interaction);
            // Emission is not included in computation as we compute direct lighting during each bounce
            // and we want to avoid sampling the same surface again if it was reached on the next bounce.
            // However, emission contribution is included for bounce 0 as no direct lighting was done prior.
            if (bounces == 0) {
                if (hit)
                    li += throughput * interaction.Le();
                // else
                // li += scene.Le();
            }

            // Break at max depth or if we hit nothing
            if (!hit || bounces >= maxDepth)
                break;

            BSDF bsdf;
            interaction.computeScattering(&bsdf);

            // Add direct lighting contribution with multiple importance sampling
            tkFloat lightPdf;
            auto light = getLightByDist(scene, sampler, *lightDist, &lightPdf);
            li += throughput * miSampleLd(interaction, scene, light, sampler) / lightPdf;

            // Spawn ray for next bounce
            Vec3f wi;
            tkFloat pdf;
            BxDFType sampledType;
            tkSpectrum f = bsdf.sample(interaction.wo, &wi, sampler.nextVector(), &pdf, &sampledType);
            if (pdf == 0 || f.isBlack())
                break;
            throughput *= f * std::abs(dot(wi, interaction.n)) / pdf;
            ray = interaction.spawnRayTo(wi);
        }
        return li;
    }
}  // namespace TK
