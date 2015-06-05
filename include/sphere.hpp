#pragma once

#include <object.hpp>
#include <vec3.hpp>
#include <ray.hpp>

namespace raytracer{

class Sphere: public Object
{
public:
    Sphere(Vec3 center, double radius, Vec3 color);
    virtual double intersect(const Ray& ray) const override;
protected:
    Vec3 center;
    double radius;
};

}
