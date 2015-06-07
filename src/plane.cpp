#include <plane.hpp>

#include <iostream>

namespace raytracer{

Plane::Plane(Vec3 normal, double dist, Vec3 color, double reflection, double diffuse, double spec, double light)
    : Object(normal, color, reflection, diffuse, spec, 0, light)
    , dist(dist)
{
}

double Plane::intersect(const Ray& ray, Vec3& normal) const
{
    //std::cout << "inter plane" << std::endl;
    //std::cout << ray.direction.z << std::endl;
    //std::cout << pos.z << std::endl;
    double Vd = pos.dot(ray.direction);
    //std::cout << Vd << std::endl;
    if (Vd == 0)
    {
        //std::cout << "parallel" << std::endl;
        // parallel, no intersection
        return 0;
    }

    double V0 = -(pos.dot(ray.origin) + dist);
    double t = V0/Vd;

    if (t < 0)
    {
        // intersection behind origin
        //std::cout << "behind" << std::endl;
        return 0;
    }

    if (Vd > 0)
    {
        normal.x = -pos.x;
        normal.y = -pos.y;
        normal.z = -pos.z;
    }
    else
    {
        normal = pos;
    }
    //std::cout << "inter at " << t << std::endl;
    return t;
}

Vec3 Plane::getRandPoint(std::default_random_engine &rng) const
{
    std::cout << "NOT IMPLEMENTED YET" << std::endl;
    return Vec3();
}

double Plane::getMinPos(const Vec3 axis) const
{
    if(axis.x != 0)
    {
        return pos.x - dimensions.x;
    }
    else if(axis.y != 0)
    {
        return pos.y - dimensions.y;
    }
    else if(axis.z != 0)
    {
        return pos.z - dimensions.z;
    }
}

double Plane::getMaxPos(const Vec3 axis) const
{
    if(axis.x != 0)
    {
        return pos.x + dimensions.x;
    }
    else if(axis.y != 0)
    {
        return pos.y + dimensions.y;
    }
    else if(axis.z != 0)
    {
        return pos.z + dimensions.z;
    }

}

}
