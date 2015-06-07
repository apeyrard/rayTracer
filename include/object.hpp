#pragma once

#include <vec3.hpp>
#include <ray.hpp>
#include <random>

namespace raytracer{

class Object
{
public:
    Object(Vec3 pos, Vec3 color, double reflection=0.0, double diffuse=1.0, double spec=0.0, double refr=0.0, double light=0.0);
    virtual ~Object();
    virtual double intersect(const Ray& ray, Vec3& normal) const = 0;
    virtual Vec3 getRandPoint(std::default_random_engine &rng) const = 0;
    Vec3 color;
    double reflection;
    double diffuse;
    double spec;
    double refr;
    double light;
protected:
    Vec3 pos;
};

}
