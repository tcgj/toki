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
    /*
    void SamplerIntegrator::render(const Scene& scene) {
        preprocess(scene);

        // Split image into tiles 16px x 16px wide to process in parallel
        int tileSize = 16;
        Vec2i res = camera->image->resolution;
        Vec2i numTiles((res.x + tileSize - 1) / tileSize, (res.y + tileSize - 1) / tileSize);

        Parallel::dispatch2D(numTiles, 1, [&](Vec2i tile) {
            // Calculate tile bounds
            int x0 = tile.x * tileSize;
            int x1 = std::min(x0 + tileSize, res.x);
            int y0 = tile.y * tileSize;
            int y1 = std::min(y0 + tileSize, res.y);

            // Get sampler clone
            std::unique_ptr<Sampler> localSampler = sampler->getClone();

            for (int y = y0; y < y1; ++y) {
                for (int x = x0; x < x1; ++x) {
                    Point2i pix(x, y);
                    localSampler->setPixel(pix);

                    tkSpectrum li;
                    do {
                        CameraSample cameraSample = localSampler->getCameraSample(pix);

                        Ray ray;
                        camera->generateRay(cameraSample, &ray);
                        li += Li(scene, ray, *localSampler);

                        // TODO: Check if radiance is valid
                    } while (localSampler->nextSample());

                    li /= localSampler->samplesPerPixel;
                    camera->image->updatePixelColor(pix, li);
                    // Vec3f col = camera->image->getPixelColor(pix);
                    // printf("Pixel [%d, %d]: [%f, %f, %f]\n", pix.x, pix.y, col.x, col.y, col.z);
                }
            }
        });
        camera->image->write();
    }
    */

    tkSpectrum Integrator::specularReflectedLi(const SurfaceInteraction& interaction,
                                                      const Scene& scene, const Ray& r, Sampler& sampler,
                                                      int depth) const {
        Vec3f wo = interaction.wo;
        Vec3f wi;
        tkFloat pdf;
        BxDFType type = BxDFType(BXDF_SPECULAR | BXDF_REFLECTIVE);
        tkSpectrum f = interaction.bsdf->sample(wo, &wi, sampler.nextVector(), &pdf, 0, type);
        tkFloat cosTheta = std::abs(dot(interaction.n, wi));
        if (pdf == 0 || f.isBlack() || cosTheta == 0)
            return 0;
        Ray reflectedRay = interaction.spawnRayTo(wi);
        return f * Li(scene, reflectedRay, sampler, depth + 1) * cosTheta / pdf;
    }

    tkSpectrum Integrator::specularRefractedLi(const SurfaceInteraction& interaction,
                                                      const Scene& scene, const Ray& r, Sampler& sampler,
                                                      int depth) const {
        // Not implemented yet
        return 0;
    }

    std::shared_ptr<Light> getLightByDist(const Scene& scene, Sampler& sampler, const Distribution& dist,
                                          tkFloat* pdf) {
        tkFloat lightIndex = dist.sampleDiscrete(sampler.nextFloat(), pdf);

        return scene.m_Lights[lightIndex];
    }

    tkSpectrum miSampleLd(const SurfaceInteraction& ref, const Scene& scene,
                          const std::shared_ptr<Light>& light, Sampler& sampler) {
        tkSpectrum ld;

        Vec3f wi;
        tkFloat lightPdf;
        tkFloat scatterPdf;
        OcclusionChecker occCheck;

        // Sampling the light
        tkSpectrum lightLd = light->sample(ref, &wi, sampler.nextVector(), &lightPdf, &occCheck);
        if (lightPdf > 0 && !lightLd.isBlack()) {
            tkSpectrum f = ref.bsdf->evaluate(ref.wo, wi);
            if (!f.isBlack() && occCheck.notOccluded(scene)) {
                tkFloat weight = 1;
                // If light is delta, don't use MIS
                if (!light->isDelta()) {
                    scatterPdf = ref.bsdf->getPdf(ref.wo, wi);
                    weight = powerHeuristic(1, lightPdf, 1, scatterPdf);
                }

                ld += f * lightLd * weight * std::abs(dot(wi, ref.n)) / lightPdf;
            }
        }

        // Sampling the BSDF
        if (light->isDelta()) {
            return ld;
        }

        BxDFType sampledType;
        tkSpectrum f = ref.bsdf->sample(ref.wo, &wi, sampler.nextVector(), &scatterPdf, &sampledType);
        if (scatterPdf > 0 && !f.isBlack()) {
            tkFloat weight = 1;
            // If specular(delta) bxdf sampled, don't use MIS
            if (!(sampledType & BXDF_SPECULAR)) {
                lightPdf = light->getPdf(ref, wi);
                // Bypass trying to trace light if pdf is 0
                if (lightPdf == 0)
                    return ld;
                weight = powerHeuristic(1, scatterPdf, 1, lightPdf);
            }
            SurfaceInteraction lightInteraction;
            Ray r = ref.spawnRayTo(wi);
            tkSpectrum scatterLd;
            if (scene.intersect(r, &lightInteraction) && lightInteraction.primitive->getLight() == light)
                scatterLd += lightInteraction.Le();

            if (!scatterLd.isBlack()) {
                ld += f * scatterLd * weight * std::abs(dot(wi, ref.n)) / scatterPdf;
            }
        }

        return ld;
    }
}  // namespace TK
