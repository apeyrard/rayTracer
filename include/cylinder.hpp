#pragma once

#include <object.hpp>
#include <vec3.hpp>
#include <ray.hpp>
#include <plane.hpp>

namespace raytracer{

class Cylinder: public Object
{
public:
    Cylinder(Vec3 center, Vec3 dir, double length, double radius, Vec3 color, double reflection=0.0, double diffuse=0.0, double spec=0.0, double refr=0.0, double light=0.0);
    ~Cylinder();
    virtual Vec3 getRandPoint(std::default_random_engine &rng) const override;
    virtual double intersect(const Ray& ray, Vec3& normal) const override;
    virtual double getMinPos(const Vec3 axis) const override;
    virtual double getMaxPos(const Vec3 axis) const override;
protected:
    double radius;
    double length;
    Plane* bottom;
    Plane* top;

    Vec3 direction;
};

}
