#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"
#include "camera.hpp"

namespace TK {
    class Sampler {
    public:
        Sampler(int64_t samplesPerPixel) : samplesPerPixel(samplesPerPixel) {}
        virtual ~Sampler() = default;

        virtual void setPixel(const Point2i& pixelCoord);
        CameraSample getCameraSample(const Point2i& pixelCoord);
        virtual void requestFloats(int count);
        virtual void requestVectors(int count);
        virtual tkFloat nextFloat() = 0;
        virtual Vec2f nextVector() = 0;
        virtual bool nextSample();
        virtual std::unique_ptr<Sampler> getClone() = 0;

        int64_t samplesPerPixel;

    protected:
        // --Sampler state values--
        Point2i currentPixel;
        int64_t currentSample;
        int currentFloatSet;
        int currentVectorSet;
        std::vector<int> floatSetSizes;
        std::vector<int> vectorSetSizes;
        std::vector<std::vector<tkFloat>> floatSet;
        std::vector<std::vector<Vec2f>> vectorSet;
    };

    inline void Sampler::setPixel(const Point2i& pixelCoord) {
        currentPixel = pixelCoord;
        currentSample = 0;
        currentFloatSet = 0;
        currentVectorSet = 0;
    }

    inline CameraSample Sampler::getCameraSample(const Point2i& pixelCoord) {
        CameraSample ret;
        ret.imgCoord = (Point2f)pixelCoord + nextVector();
        ret.lens = nextVector();
        return ret;
    }

    inline void Sampler::requestFloats(int count) {
        floatSetSizes.push_back(count);
        floatSet.push_back(std::vector<tkFloat>(count * samplesPerPixel));
    }

    inline void Sampler::requestVectors(int count) {
        vectorSetSizes.push_back(count);
        vectorSet.push_back(std::vector<Vec2f>(count * samplesPerPixel));
    }

    inline bool Sampler::nextSample() {
        currentSample++;
        currentFloatSet = 0;
        currentVectorSet = 0;
        return currentSample < samplesPerPixel;
    }
}  // namespace TK
