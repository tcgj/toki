#include "ao.hpp"

#include "core/sampler.hpp"

namespace TK {
    tkSpectrum AOIntegrator::Li(const Scene& scene, const Ray& r, Sampler& sampler, int depth) const {
        SurfaceInteraction interaction;
        if (!scene.intersect(r, &interaction))
            return 0;

        int clearCount = 0;
        for (int i = 0; i < m_NumSamples; ++i) {
            Vec3f w = uniformSphereSample(sampler.nextFloat(), sampler.nextFloat());
            tkFloat dotP = dot(w, interaction.n);
            if (dotP < 0)
                w = -w;

            Ray test = interaction.spawnRayTo(w);
            test.tMax = m_MaxDist;
            SurfaceInteraction its;
            if (dotP == 0 || !scene.intersect(test, &its))
                clearCount++;
        }
        return (tkFloat)clearCount / m_NumSamples;
    }
}  // namespace TK