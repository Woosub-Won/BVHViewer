#include "Skeleton.h"

void Skeleton::clear()
{
    if (mRoot)
    {
        deleteJoint(mRoot);
        mRoot = nullptr;
    }
    mChannelOrder.clear();
}

void Skeleton::deleteJoint(Joint* joint)
{
    for (Joint* child : joint->getChildren())
        deleteJoint(child);
    delete joint;
}