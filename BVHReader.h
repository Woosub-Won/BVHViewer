#pragma once
#ifndef __BVH_READER_H__
#define __BVH_READER_H__

#include "Skeleton.h"
#include "Motion.h"
#include <string>
#include <fstream>

class BVHReader
{
public:
    // BVH 파일을 읽어 skeleton과 motion을 채운다. 성공 시 true 반환.
    bool read(const std::string& filename, Skeleton& skeleton, Motion& motion);

private:
    void   parseHierarchy(std::ifstream& file, Skeleton& skeleton);
    Joint* parseJoint(std::ifstream& file, Skeleton& skeleton, Joint* parent);
    void   parseChannels(std::ifstream& file, int count, Joint* joint, Skeleton& skeleton);
    void   parseMotion(std::ifstream& file, Skeleton& skeleton, Motion& motion);

    Channel toChannel(const std::string& str);
};

#endif