#pragma once

#include "system/toki.hpp"
#include "camera.hpp"

namespace TK {
    class Sampler {
    public:
        Sampler(int64_t samplesPerPixel) : m_SamplesPerPixel(samplesPerPixel) {}

        virtual ~Sampler() = default;

        virtual void setPixel(int x, int y);

        CameraSample getCameraSample();

        virtual void requestFloats(int count);

        virtual void requestVectors(int count);

        virtual tkFloat nextFloat() = 0;

        virtual Vec2f nextVector() = 0;

        virtual bool nextSample();

        virtual std::unique_ptr<Sampler> getClone() = 0;

        int64_t m_SamplesPerPixel;

    protected:
        // --Sampler state values--
        Point2i m_CurrentPixel;
        int64_t m_CurrentSample;
        int m_CurrentFloatSet;
        int m_CurrentVectorSet;
        std::vector<int> m_FloatSetSizes;
        std::vector<int> m_VectorSetSizes;
        std::vector<std::vector<tkFloat>> m_FloatSet;
        std::vector<std::vector<Vec2f>> m_VectorSet;
    };

    inline void Sampler::setPixel(int x, int y) {
        m_CurrentPixel = Point2i(x, y);
        m_CurrentSample = 0;
        m_CurrentFloatSet = 0;
        m_CurrentVectorSet = 0;
    }

    inline CameraSample Sampler::getCameraSample() {
        return { nextVector(), nextVector() };
    }

    inline void Sampler::requestFloats(int count) {
        m_FloatSetSizes.push_back(count);
        m_FloatSet.push_back(std::vector<tkFloat>(count * m_SamplesPerPixel));
    }

    inline void Sampler::requestVectors(int count) {
        m_VectorSetSizes.push_back(count);
        m_VectorSet.push_back(std::vector<Vec2f>(count * m_SamplesPerPixel));
    }

    inline bool Sampler::nextSample() {
        m_CurrentSample++;
        m_CurrentFloatSet = 0;
        m_CurrentVectorSet = 0;
        return m_CurrentSample < m_SamplesPerPixel;
    }
}  // namespace TK
