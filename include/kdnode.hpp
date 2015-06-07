#pragma once

#include <object.hpp>

namespace raytracer{

class KDNode
{
    KDNode();
    std::vector<Object*> objects;
    double splitPos;
    Vec3 axis;
    KDNode* front;
    KDNode* behind;

    Vec3 dimensions;
};

}
