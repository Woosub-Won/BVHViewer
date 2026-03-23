#include "BVHReader.h"
#include <iostream>

// ----------------------------------------------------------------
bool BVHReader::read(const std::string& filename, Skeleton& skeleton, Motion& motion)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "[BVHReader] Cannot open file: " << filename << std::endl;
        return false;
    }

    skeleton.clear();
    motion.clear();

    std::string token;
    while (file >> token)
    {
        if (token == "HIERARCHY")
        {
            parseHierarchy(file, skeleton);
        }
        else if (token == "MOTION")
        {
            parseMotion(file, skeleton, motion);
        }
    }

    motion.setSkeleton(&skeleton);
    return true;
}

// ----------------------------------------------------------------
void BVHReader::parseHierarchy(std::ifstream& file, Skeleton& skeleton)
{
    std::string token;
    while (file >> token)
    {
        if (token == "ROOT")
        {
            Joint* root = parseJoint(file, skeleton, nullptr);
            skeleton.setRoot(root);
            return;
        }
    }
}

// ----------------------------------------------------------------
Joint* BVHReader::parseJoint(std::ifstream& file, Skeleton& skeleton, Joint* parent)
{
    std::string name;
    file >> name;

    Joint* joint = new Joint(name, parent);

    std::string token;
    while (file >> token)
    {
        if (token == "{")
        {
            // 블록 시작
        }
        else if (token == "}")
        {
            // 블록 종료
            break;
        }
        else if (token == "OFFSET")
        {
            float x, y, z;
            file >> x >> y >> z;
            joint->setOffset(vec3(x, y, z));
        }
        else if (token == "CHANNELS")
        {
            int count;
            file >> count;
            parseChannels(file, count, joint, skeleton);
        }
        else if (token == "JOINT")
        {
            Joint* child = parseJoint(file, skeleton, joint);
            joint->addChild(child);
        }
        else if (token == "End") // End Site
        {
            file >> token; // "Site"

            Joint* endSite = new Joint(name + "_end", joint);
            endSite->setIsEndSite(true);

            file >> token; // "{"
            while (file >> token)
            {
                if (token == "OFFSET")
                {
                    float x, y, z;
                    file >> x >> y >> z;
                    endSite->setOffset(vec3(x, y, z));
                }
                else if (token == "}")
                {
                    break;
                }
            }

            joint->addChild(endSite);
        }
    }

    return joint;
}

// ----------------------------------------------------------------
void BVHReader::parseChannels(std::ifstream& file, int count,
    Joint* joint, Skeleton& skeleton)
{
    std::vector<Channel> channels;
    channels.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        std::string name;
        file >> name;
        Channel ch = toChannel(name);
        channels.push_back(ch);

        // Motion이 인덱싱할 전체 채널 순서에 누적
        skeleton.getChannelOrder().push_back(ch);
    }

    joint->setChannels(channels);
}

// ----------------------------------------------------------------
void BVHReader::parseMotion(std::ifstream& file, Skeleton& skeleton, Motion& motion)
{
    std::string token;

    file >> token;   // "Frames:"
    int frameCount;
    file >> frameCount;
    motion.setFrameCount(frameCount);

    file >> token >> token; // "Frame" "Time:"
    double frameTime;
    file >> frameTime;
    motion.setFrameTime(frameTime);

    int channelCount = static_cast<int>(skeleton.getChannelOrder().size());

    for (int f = 0; f < frameCount; ++f)
    {
        motion.getFrame(f).resize(channelCount);
        for (int c = 0; c < channelCount; ++c)
            file >> motion.getFrame(f)[c];
    }
}

// ----------------------------------------------------------------
Channel BVHReader::toChannel(const std::string& str)
{
    if (str == "Xposition") return Channel::Xposition;
    if (str == "Yposition") return Channel::Yposition;
    if (str == "Zposition") return Channel::Zposition;
    if (str == "Xrotation") return Channel::Xrotation;
    if (str == "Yrotation") return Channel::Yrotation;
    if (str == "Zrotation") return Channel::Zrotation;

    std::cerr << "[BVHReader] Unknown channel: " << str << std::endl;
    return Channel::Xposition;
}