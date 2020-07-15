#include "integrator.hpp"

#include "ray.hpp"
#include "interaction.hpp"
#include "scattering.hpp"
#include "spectrum.hpp"

namespace TK {
    void SamplerIntegrator::render(const Scene &scene) {
        preprocess(scene);

        // implement parallel processing here
        // iterate through tiles of image in parallel
        // render each tile of the image
        // combine output to form image
        // write image to file
    }

    tkSpectrum SamplerIntegrator::computeReflectionLi(const SurfaceInteraction &interaction,
                                    const Scene &scene, const Ray &r,
                                    Sampler &sampler, tkUInt depth) const {
        tkVec3f wo = interaction.wo;
        tkVec3f wi;
        tkFloat pdf;
        tkSpectrum f = 0; // Sample scattering function
        Ray reflectedRay = interaction.spawnRayTo(wi);
        tkFloat cosTheta = std::abs(dot(interaction.n, wi));
        return f * computeLi(scene, reflectedRay, sampler, depth + 1) * cosTheta / pdf;
    }

    tkSpectrum SamplerIntegrator::computeRefractionLi(const SurfaceInteraction &interaction,
                                    const Scene &scene, const Ray &r,
                                    Sampler &sampler, tkUInt depth) const {
        // Not implemented yet
        return 0;
    }
} // namespace TK
