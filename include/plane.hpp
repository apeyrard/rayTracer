#pragma once

#include <object.hpp>
#include <vec3.hpp>
#include <ray.hpp>

namespace raytracer{

class Plane: public Object
{
public:
    Plane(Vec3 normal, double dist, Vec3 color, double reflection=0.0, double diffuse=1.0, double spec=0.0, double light=0.0);
    virtual Vec3 getRandPoint(std::default_random_engine &rng) const override;
    virtual double getMinPos(const Vec3 axis) const override;
    virtual double getMaxPos(const Vec3 axis) const override;
    virtual double intersect(const Ray& ray, Vec3& normal) const override;
private:
    double dist;
};

}
