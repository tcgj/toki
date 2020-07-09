#include "integrator.hpp"

namespace TK {
    void SamplerIntegrator::render(const Scene &scene) {
        preprocess(scene);

        // implement parallel processing here
        // iterate through tiles of image in parallel
        // render each tile of the image
        // combine output to form image
        // write image to file
    }
} // namespace TK
