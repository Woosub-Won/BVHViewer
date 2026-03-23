#include "Joint.h"

Joint::Joint(const std::string& name, Joint* parent)
    : mName(name), mParent(parent)
{
}

void Joint::addChild(Joint* child)
{
    mChildren.push_back(child);
}
