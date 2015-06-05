#pragma once

#include <vec3.hpp>

namespace raytracer{

class Ray
{
public:
    Ray(Vec3 origin, Vec3 direction);
    Vec3 origin;
    Vec3 direction;
};

}
