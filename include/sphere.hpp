#pragma once

#include <object.hpp>
#include <vec3.hpp>
#include <ray.hpp>

namespace raytracer{

class Sphere: public Object
{
public:
    Sphere(Vec3 center, double radius, Vec3 color, double reflection=0.0, double diffuse=1.0, double spec=0.0);
    virtual double intersect(const Ray& ray, Vec3& normal) const override;
protected:
    double radius;
};

}
