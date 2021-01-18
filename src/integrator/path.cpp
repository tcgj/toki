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
        switch (m_Strategy) {
            case POWER:
                m_LightDist = lightPowerDistribution(scene);
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
        SurfaceInteraction its;

        for (int bounces = 0;; ++bounces) {
            bool hit = scene.intersect(ray, its);
            // Emission is not included in computation as we compute direct lighting during each bounce
            // and we want to avoid sampling the same surface again if it was reached on the next bounce.
            // However, emission contribution is included for bounce 0 as no direct lighting was done prior.
            if (bounces == 0) {
                if (hit)
                    li += throughput * its.Le();
                // else
                // li += scene.Le();
            }

            // Break at max depth or if we hit nothing
            if (!hit || bounces >= m_MaxDepth)
                break;

            BSDF bsdf = its.getBSDF();

            // Add direct lighting contribution with multiple importance sampling
            if (m_LightDist != nullptr) {
                tkFloat lightPdf;
                auto light = getLightByDist(scene, sampler, *m_LightDist, &lightPdf);
                li += throughput * miSampleLd(its, scene, light, sampler) / lightPdf;
            }

            // Spawn ray for next bounce
            BSDFSample bs = bsdf.sample(its.wo, sampler.nextVector());
            if (!bs || !bs.f)
                break;
            throughput *= bs.f * std::abs(dot(bs.wi, its.n)) / bs.pdf;
            ray = its.spawnRayTo(bs.wi);
        }
        return li;
    }
}  // namespace TK
