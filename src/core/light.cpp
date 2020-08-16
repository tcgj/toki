#include "light.hpp"

namespace TK {
    std::unique_ptr<Distribution> lightPowerDistribution(const Scene &scene) {
        if (scene.lights.empty())
            return 0;

        std::vector<tkFloat> powers;
        for (const auto &light : scene.lights) {
            powers.push_back(light->power().luminance());
        }
        return std::make_unique<Distribution>(powers);
    }
} // namespace TK
