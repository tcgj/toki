#include "integrator.hpp"

#include "ray.hpp"
#include "interaction.hpp"
#include "scattering.hpp"
#include "spectrum.hpp"
#include "sampler.hpp"

namespace TK {
    void SamplerIntegrator::render(const Scene &scene) {
        preprocess(scene);

        // implement parallel processing here
        // iterate through tiles of image in parallel
        // render each tile of the image
        // combine output to form image
        // write image to file
    }

    tkSpectrum SamplerIntegrator::computeReflectedLi(const SurfaceInteraction &interaction,
                                    const Scene &scene, const Ray &r,
                                    Sampler &sampler, tkUInt depth) const {
        tkVec3f wo = interaction.wo, wi;
        tkFloat pdf;
        BxDFType type = BxDFType(BXDF_SPECULAR | BXDF_REFLECTIVE);
        // TODO: Fix sampler interface for continuous sampling
        tkVec2f sample;
        sampler.sample2D(&sample, 1, 1);
        tkSpectrum f = interaction.scattering->sample(wo, &wi, sample, &pdf, type);
        Ray reflectedRay = interaction.spawnRayTo(wi);
        tkFloat cosTheta = std::abs(dot(interaction.n, wi));
        return f * computeLi(scene, reflectedRay, sampler, depth + 1) * cosTheta / pdf;
    }

    tkSpectrum SamplerIntegrator::computeRefractedLi(const SurfaceInteraction &interaction,
                                    const Scene &scene, const Ray &r,
                                    Sampler &sampler, tkUInt depth) const {
        // Not implemented yet
        return 0;
    }
} // namespace TK
