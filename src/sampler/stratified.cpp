#include "stratified.hpp"

#include "core/random.hpp"

namespace TK {
    StratifiedSampler::StratifiedSampler(int xCount, int yCount, int dimensions)
        : Sampler(xCount * yCount), m_XCount(xCount), m_YCount(yCount) {
        // Pre-initialize samples with a max dimension count
        for (int i = 0; i < dimensions; ++i) {
            Sampler::requestFloats(1);
            Sampler::requestVectors(1);
        }
    }

    void generateFloatSamples(tkFloat *samples, int count) {
        tkFloat invCount = (tkFloat)1 / count;
        for (int i = 0; i < count; ++i) {
            samples[i] = (i + Random::nextFloat()) * invCount;
        }
    }

    void generateVectorSamples(Vec2f *samples, int xCount, int yCount) {
        tkFloat invX = (tkFloat)1 / xCount;
        tkFloat invY = (tkFloat)1 / yCount;
        Vec2f *currSample = samples;
        for (int y = 0; y < yCount; ++y) {
            for (int x = 0; x < xCount; ++x) {
                currSample->x = (x + Random::nextFloat()) * invX;
                currSample->y = (y + Random::nextFloat()) * invY;
                currSample++;
            }
        }
    }

    template <typename T>
    void randomizeSamples(T *samples, int count) {
        for (int i = 0; i < count; ++i) {
            // Sanity check to ensure o never goes out of bounds for whatever reason
            int o = std::min(count - 1, i + static_cast<int>(Random::nextFloat() * (count - i)));
            std::swap(samples[i], samples[o]);
        }
    }

    void StratifiedSampler::setPixel(int x, int y) {
        // Set up float set
        for (auto &f : m_FloatSet) {
            generateFloatSamples(f.data(), m_SamplesPerPixel);
            randomizeSamples(f.data(), m_SamplesPerPixel);
        }
        // Set up vector set
        for (auto &v : m_VectorSet) {
            generateVectorSamples(v.data(), m_XCount, m_YCount);
            randomizeSamples(v.data(), m_XCount * m_YCount);
        }
        Sampler::setPixel(x, y);
    }

    tkFloat StratifiedSampler::nextFloat() {
        if (m_CurrentFloatSet < m_FloatSet.size()) {
            return m_FloatSet[m_CurrentFloatSet++][m_CurrentSample];
        } else {
            return Random::nextFloat();
        }
    }

    Vec2f StratifiedSampler::nextVector() {
        if (m_CurrentVectorSet < m_VectorSet.size()) {
            return m_VectorSet[m_CurrentVectorSet++][m_CurrentSample];
        } else {
            return Vec2f(Random::nextFloat(), Random::nextFloat());
        }
    }

    std::unique_ptr<Sampler> StratifiedSampler::getClone() {
        return std::make_unique<StratifiedSampler>(*this);
    }
}  // namespace TK
