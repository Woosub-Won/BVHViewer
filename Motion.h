#pragma once
#ifndef __MOTION_H__
#define __MOTION_H__

#include "Skeleton.h"
#include <vector>

class Motion
{
public:
    Motion() = default;
    ~Motion() = default;

    void setSkeleton(Skeleton* skeleton) { mSkeleton = skeleton; }
    Skeleton* getSkeleton() const { return mSkeleton; }

    void   setFrameCount(int count) { mFrameData.resize(count); }
    void   setFrameTime(double time) { mFrameTime = time; }
    int    getFrameCount() const { return static_cast<int>(mFrameData.size()); }
    double getFrameTime()  const { return mFrameTime; }

    // 특정 프레임의 채널 값 벡터 (쓰기용)
    std::vector<float>& getFrame(int frame) { return mFrameData[frame]; }
    // 특정 프레임의 채널 값 벡터 (읽기용)
    const std::vector<float>& getFrame(int frame) const { return mFrameData[frame]; }

    // 전체 프레임 데이터
    const std::vector<std::vector<float>>& getAllFrames() const { return mFrameData; }

    void clear();

private:
    Skeleton* mSkeleton = nullptr;

    double mFrameTime = 0.0;

    // mFrameData[frame][channelIndex]
    // channelIndex는 mSkeleton->getChannelOrder() 기준
    std::vector<std::vector<float>> mFrameData;
};

#endif 