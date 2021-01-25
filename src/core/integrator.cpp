#include "integrator.hpp"

#include "ray.hpp"
#include "interaction.hpp"
#include "camera.hpp"
#include "bsdf.hpp"
#include "spectrum.hpp"
#include "sampler.hpp"
#include "random.hpp"
#include "scene.hpp"
#include "region/primitive.hpp"

namespace TK {
    std::shared_ptr<Light> getLightByDist(const Scene& scene, Sampler& sampler, const Distribution& dist,
                                          tkFloat* pdf) {
        tkFloat lightIndex = dist.sampleDiscrete(sampler.nextFloat(), pdf);

        return scene.m_Lights[lightIndex];
    }

    tkSpectrum miSampleLd(const SurfaceInteraction& ref, const Scene& scene,
                          const std::shared_ptr<Light>& light, Sampler& sampler) {
        tkSpectrum ld;
        BSDF bsdf = ref.getBSDF();

        // Sampling the light
        LightSample ls = light->sample(ref, sampler.nextVector());
        if (ls && ls.l) {
            tkSpectrum f = bsdf.evaluate(ref.wo, ls.wi);
            if (f && !ls.isOccluded(scene, ref)) {
                tkFloat weight = 1;
                // If light is delta, don't use MIS
                // if (!light->isDelta()) {
                    tkFloat scatterPdf = bsdf.getPdf(ref.wo, ls.wi);
                    weight = powerHeuristic(1, ls.pdf, 1, scatterPdf);
                // }

                ld += f * ls.l * weight * std::abs(dot(ls.wi, ref.n)) / ls.pdf;
            }
        }

        // Sampling the BSDF
        // if (light->isDelta()) {
            return ld;
        // }

        BxDFType sampledType;
        BSDFSample bs = bsdf.sample(ref.wo, sampler.nextVector());
        if (bs && bs.f) {
            tkFloat weight = 1;
            // If specular(delta) bxdf sampled, don't use MIS
            if (!(sampledType & BXDF_SPECULAR)) {
                tkFloat lightPdf = light->getPdf(ref, bs.wi);
                // Bypass trying to trace light if pdf is 0
                if (lightPdf == 0)
                    return ld;
                weight = powerHeuristic(1, bs.pdf, 1, lightPdf);
            }
            SurfaceInteraction lightInteraction;
            Ray r = ref.spawnRayTo(bs.wi);
            tkSpectrum scatterLd;
            if (scene.intersect(r, lightInteraction) && lightInteraction.light == light)
                scatterLd += lightInteraction.Le();

            if (scatterLd) {
                ld += bs.f * scatterLd * weight * std::abs(dot(bs.wi, ref.n)) / bs.pdf;
            }
        }

        return ld;
    }
}  // namespace TK
