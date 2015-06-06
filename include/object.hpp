#pragma once

#include <vec3.hpp>
#include <ray.hpp>

namespace raytracer{

class Object
{
public:
    Object(Vec3 pos, Vec3 color, double reflection=0.0, double diffuse=1.0);
    virtual double intersect(const Ray& ray, Vec3& normal) const = 0;
    Vec3 color;
    double reflection;
    double diffuse;
protected:
    Vec3 pos;
};

}
