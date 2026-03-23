#pragma once
#ifndef __SKELETON_H__
#define __SKELETON_H__

#include "Joint.h"

class Skeleton
{
public:
    Skeleton() = default;
    ~Skeleton() { clear(); }

    void   setRoot(Joint* root) { mRoot = root; }
    Joint* getRoot() const { return mRoot; }

    // BVH 파싱 순서대로 쌓인 전체 채널 목록.
    // Motion이 frame 데이터를 인덱싱할 때 이 순서를 기준으로 삼는다.
    std::vector<Channel>& getChannelOrder() { return mChannelOrder; }
    const std::vector<Channel>& getChannelOrder() const { return mChannelOrder; }

    // Joint 트리 + 채널 순서 초기화
    void clear();

private:
    Joint* mRoot = nullptr;
    std::vector<Channel> mChannelOrder;

    void deleteJoint(Joint* joint);
};

#endif