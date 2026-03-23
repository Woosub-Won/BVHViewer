#pragma once
#ifndef __JOINT_H__
#define __JOINT_H__

#include <string>
#include <vector>
#include <vec.h>

enum class Channel 
{
    Xposition, Yposition, Zposition,
    Xrotation, Yrotation, Zrotation
};

class Joint
{
public:
    Joint(const std::string& name, Joint* parent = nullptr);
    ~Joint() = default;

    void addChild(Joint* child);

    const std::string& getName()     const { return mName; }
    Joint* getParent()   const { return mParent; }
    const std::vector<Joint*>& getChildren() const { return mChildren; }
    const vec3& getOffset()   const { return mOffset; }
    const std::vector<Channel>& getChannels() const { return mChannels; }
    bool                        isEndSite()   const { return mIsEndSite; }

    void setOffset(const vec3& offset) { mOffset = offset; }
    void setChannels(const std::vector<Channel>& ch) { mChannels = ch; }
    void setIsEndSite(bool isEnd) { mIsEndSite = isEnd; }

private:
    std::string mName;
    Joint* mParent;
    std::vector<Joint*> mChildren;

    vec3 mOffset;
    std::vector<Channel> mChannels;
    bool mIsEndSite = false;
};

#endif