#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"
#include "camera.hpp"

namespace TK {
    class Sampler {
    public:
        Sampler(tkI64 samplesPerPixel) : samplesPerPixel(samplesPerPixel) {}
        virtual ~Sampler() = default;

        virtual void setPixel(const tkPoint2i &pixelCoord);
        CameraSample getCameraSample(const tkPoint2i &pixelCoord);
        virtual void requestFloats(tkUInt count);
        virtual void requestVectors(tkUInt count);
        virtual tkFloat nextFloat() = 0;
        virtual tkVec2f nextVector() = 0;
        virtual bool nextSample();
        virtual std::unique_ptr<Sampler> getClone() = 0;


        tkI64 samplesPerPixel;
    protected:
        // --Sampler state values--
        tkPoint2i currentPixel;
        tkI64 currentSample;
        tkUInt currentFloatSet, currentVectorSet;
        std::vector<tkUInt> floatSetSizes, vectorSetSizes;
        std::vector<std::vector<tkFloat>> floatSet;
        std::vector<std::vector<tkVec2f>> vectorSet;
    };

    inline void Sampler::setPixel(const tkPoint2i &pixelCoord) {
        currentPixel = pixelCoord;
        currentSample = 0;
        currentFloatSet = 0;
        currentVectorSet = 0;
    }

    inline CameraSample Sampler::getCameraSample(const tkPoint2i &pixelCoord) {
        CameraSample ret;
        ret.imgCoord = (tkPoint2f)pixelCoord + nextVector();
        ret.lens = nextVector();
        return ret;
    }

    inline void Sampler::requestFloats(tkUInt count) {
        floatSetSizes.push_back(count);
        floatSet.push_back(std::vector<tkFloat>(count * samplesPerPixel));
    }

    inline void Sampler::requestVectors(tkUInt count) {
        vectorSetSizes.push_back(count);
        vectorSet.push_back(std::vector<tkVec2f>(count * samplesPerPixel));
    }

    inline bool Sampler::nextSample() {
        currentSample++;
        currentFloatSet = 0;
        currentVectorSet = 0;
        return currentSample < samplesPerPixel;
    }
} // namespace TK
