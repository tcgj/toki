#include "whitted.hpp"

#include "core/scene.hpp"
#include "core/bsdf.hpp"
#include "core/light.hpp"
#include "core/interaction.hpp"
#include "core/spectrum.hpp"
#include "core/sampler.hpp"
#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum WhittedIntegrator::Li(const Scene& scene, const Ray& r, Sampler& sampler, int depth) const {
        tkSpectrum li;
        SurfaceInteraction its;
        if (!scene.intersect(r, its)) {
            // TODO: get radiance contribution from
            // light source along the ray if any (sky light etc)
            return li;
        }

        BSDF bsdf = its.getBSDF();

        Vec3f normal = its.n;
        Vec3f wo = its.wo;

        // Add emission contribution if interaction with light
        li += its.Le();

        // Add contribution of lighting on surface point
        for (const auto& light : scene.m_Lights) {
            // Ignore non-delta light source as whitted integrator does not handle
            // if (!light->isDelta())
            // continue;

            LightSample ls = light->sample(its, sampler.nextVector());
            if (!ls || !ls.l)
                continue;

            tkSpectrum f = bsdf.evaluate(wo, ls.wi);
            if (f && !ls.isOccluded(scene, its))
                li += f * ls.l * std::abs(dot(ls.wi, normal)) / ls.pdf;
        }

        // Spawn secondary rays from intersection point
        // if depth is under max depth
        if (depth < m_MaxDepth - 1) {
            BSDFSample bs = bsdf.sample(wo, sampler.nextVector(), BxDFType(BXDF_SPECULAR | BXDF_REFLECTIVE));
            if (bs && bs.f) {
                tkFloat cosTheta = std::abs(dot(its.n, bs.wi));
                Ray reflectedRay = its.spawnRayTo(bs.wi);
                li += bs.f * Li(scene, reflectedRay, sampler, depth + 1) * cosTheta / bs.pdf;
            }
            // li += specularRefractedLi(its, scene, r, sampler, depth);
        }

        return li;
    }
}  // namespace TK
