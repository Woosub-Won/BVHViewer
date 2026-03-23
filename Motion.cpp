#include "Motion.h"

void Motion::clear()
{
    mSkeleton = nullptr;
    mFrameTime = 0.0;
    mFrameData.clear();
}