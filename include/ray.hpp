#pragma once

#include <vec3.hpp>

namespace raytracer{

class Ray
{
public:
    Ray(Vec3 origin, Vec3 direction, int depth=0, double refr=1.0);
    Vec3 origin;
    Vec3 direction;
    int depth;
    double refr;
};

}
