#include "integrator.hpp"

#include "ray.hpp"
#include "interaction.hpp"
#include "camera.hpp"
#include "scattering.hpp"
#include "spectrum.hpp"
#include "sampler.hpp"
#include "parallel.hpp"
#include "random.hpp"

namespace TK {
    void SamplerIntegrator::render(const Scene &scene) {
        preprocess(scene);

        // Split image into tiles 16px x 16px wide to process in parallel
        tkInt tileSize = 16;
        tkInt numSamplesPerPixel = 16;
        tkVec2i res = camera->image->resolution;
        tkVec2i numTiles((res.x + tileSize - 1) / tileSize,
                         (res.y + tileSize - 1) / tileSize);

        parallelFor2D(numTiles, 1, [&](tkVec2i tile) {
            // Calculate tile bounds
            tkInt x0 = tile.x * tileSize;
            tkInt x1 = std::min(x0 + tileSize, res.x);
            tkInt y0 = tile.y * tileSize;
            tkInt y1 = std::min(y0 + tileSize, res.y);

            for (tkInt y = y0; y < y1; ++y) {
                for (tkInt x = x0; x < x1; ++x) {
                    tkPoint2i pix(x, y);

                    tkSpectrum li;
                    for (tkInt i = 0; i < numSamplesPerPixel; ++i) {
                        CameraSample cameraSample;
                        cameraSample.imgCoord = tkPoint2f(Random::nextFloat() + pix.x,
                                                          Random::nextFloat() + pix.y);

                        Ray ray;
                        camera->generateRay(cameraSample, &ray);
                        li += computeLi(scene, ray, *sampler);

                        // TODO: Check if radiance is valid
                    }

                    li /= numSamplesPerPixel;
                    camera->image->updatePixelColor(pix, li);
                    // tkVec3f col = camera->image->getPixelColor(pix);
                    // printf("Pixel [%d, %d]: [%f, %f, %f]\n", pix.x, pix.y, col.x, col.y, col.z);
                }
            }
        });
        camera->image->write();
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
        tkFloat cosTheta = std::abs(dot(interaction.n, wi));
        if (pdf == 0 || f.isBlack() || cosTheta == 0)
            return 0;
        Ray reflectedRay = interaction.spawnRayTo(wi);
        return f * computeLi(scene, reflectedRay, sampler, depth + 1) * cosTheta / pdf;
    }

    tkSpectrum SamplerIntegrator::computeRefractedLi(const SurfaceInteraction &interaction,
                                    const Scene &scene, const Ray &r,
                                    Sampler &sampler, tkUInt depth) const {
        // Not implemented yet
        return 0;
    }
} // namespace TK
