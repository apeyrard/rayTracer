#include <sphere.hpp>
#include <cmath>

#include <iostream>

namespace raytracer{

Sphere::Sphere(Vec3 pos, double radius, Vec3 color, double reflection, double diffuse)
    : Object(pos, color, reflection, diffuse)
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
        normal = (intersec - pos)/radius;
        return t1;
    }

    return 0; // no positive solution : no intersection in desired direction
}

}
