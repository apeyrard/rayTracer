#pragma once

#include <object.hpp>
#include <vec3.hpp>
#include <ray.hpp>
#include <plane.hpp>

namespace raytracer{

class Box: public Object
{
public:
    Box(Vec3 center, Vec3 orientation, Vec3 dimensions, Vec3 color, double reflection=0.0, double diffuse=1.0, double spec=0.0, double refr=0.0, double light=0.0);
    virtual Vec3 getRandPoint(std::default_random_engine &rng) const override;
    virtual double intersect(const Ray& ray, Vec3& normal) const override;
protected:
    Vec3 orientation;
    Vec3 dimensions;
};

}
