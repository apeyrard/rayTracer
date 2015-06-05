#pragma once

#include <vec3.hpp>
#include <ray.hpp>

namespace raytracer{

class Object
{
public:
    Object(Vec3 pos, Vec3 color);
    virtual double intersect(const Ray& ray) const = 0;
protected:
    Vec3 pos;
    Vec3 color;
};

}
