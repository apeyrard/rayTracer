#include <sphere.hpp>
#include <cmath>
#include <iostream>

namespace raytracer{

Sphere::Sphere(Vec3 pos, double radius, Vec3 color, double reflection, double diffuse, double spec, double refr, double light)
    : Object(pos, color, reflection, diffuse, spec, refr, light)
    , radius(radius)
{
}

double Sphere::intersect(const Ray& ray, Vec3& normal) const
{
    // ray direction is normalized do A = 1
    double B = 2*(ray.direction.x * (ray.origin.x - pos.x) + ray.direction.y * (ray.origin.y - pos.y) + ray.direction.z * (ray.origin.z - pos.z));
    double C = pow((ray.origin.x - pos.x), 2) + pow((ray.origin.y - pos.y), 2) + pow((ray.origin.z - pos.z), 2) - pow(radius, 2);

    double delta = pow(B, 2) - 4*C;
    if (delta < 0)
    {
        // negative discriminat, no solution to the equation, no intersection
        return 0;
    }

    double t0 = (-B - sqrt(delta))/2;
    if (t0 > 0)
    {
        //closest intersection is t0
        Vec3 intersec = ray.origin + (ray.direction * t0);
        normal = ((intersec - pos)/radius).norm(); //TODO check if norm necessary
        return t0;
    }

    double t1 = (-B + sqrt(delta))/2;
    if (t1 > 0)
    {
        //closest intersection is t1
        Vec3 intersec = ray.origin + (ray.direction * t0);
        normal = ((intersec - pos)/radius).norm();
        return t1;
    }

    return 0; // no positive solution : no intersection in desired direction
}

Vec3 Sphere::getRandPoint(std::default_random_engine &rng) const
{
    std::normal_distribution<double> gauss(0, 1);
    //random normalized vector
    Vec3 randVec = Vec3(gauss(rng), gauss(rng), gauss(rng)).norm();
    randVec = randVec*radius + pos;
    //std::cout << randVec.x << " " << randVec.y << " " << randVec.z << " " << randVec.length() << std::endl;
    return randVec;
}

double Sphere::getMinPos(const Vec3 axis) const
{
    if(axis.x != 0)
    {
        return pos.x - radius;
    }
    else if(axis.y != 0)
    {
        return pos.y - radius;
    }
    else if(axis.z != 0)
    {
        return pos.z - radius;
    }
}

double Sphere::getMaxPos(const Vec3 axis) const
{
    if(axis.x != 0)
    {
        return pos.x + radius;
    }
    else if(axis.y != 0)
    {
        return pos.y + radius;
    }
    else if(axis.z != 0)
    {
        return pos.z + radius;
    }

}

}
