#include <box.hpp>
#include <algorithm>
#include <iostream>
#include <cstdlib>

namespace raytracer{

//TODO orientation

Box::Box(Vec3 center, Vec3 orientation, Vec3 dimensions, Vec3 color, double reflection, double diffuse, double spec, double refr, double light)
    :Object(center, color, reflection, diffuse, spec, refr, light)
    , orientation(orientation)
    , dimensions(dimensions)
{
}

Vec3 Box::getRandPoint(std::default_random_engine &rng) const
{
    //TODO
    return Vec3();
}

double Box::intersect(const Ray& ray, Vec3& normal) const
{
    double tnear = -1;
    double tfar = -1;
    Vec3 minNorm;


    double x1 = pos.x - dimensions.x;
    double x2 = pos.x + dimensions.x;
    double y1 = pos.y - dimensions.y;
    double y2 = pos.y + dimensions.y;
    double z1 = pos.z - dimensions.z;
    double z2 = pos.z + dimensions.z;

    //dir x
    if (ray.direction.x == 0)
    {
        //std::cout << "in x" << std::endl;
        //parallel

        //outside
        if ((ray.origin.x < std::min(x1, x2)) or (ray.origin.x > std::max(x1, x2)))
        {
            return 0;
        }
    }
    else
    {
        double t1 = (x1 - ray.origin.x)/ray.direction.x;
        double t2 = (x2 - ray.origin.x)/ray.direction.x;
        if (t1 < 0)
        {
            t1 = -1;
        }
        if (t2 < 0)
        {
            t2= -1;
        }

        if (t1 > t2)
        {
            double tmp = t1;
            t1 = t2;
            t2 = tmp;
        }

        if ((t1 > tnear) or (tnear == -1))
        {
            tnear = t1;
        }

        if ((t2 < tfar) or (tfar == -1))
        {
            tfar = t2;
        }

        if (tnear > tfar)
        {
            //missed
            return 0;
        }

        if (tfar < 0)
        {
            //behind
            return 0;
        }
    }

   //dir y
    if (ray.direction.y == 0)
    {
        //std::cout << "in y" << std::endl;
        //parallel

        //outside
        if ((ray.origin.y < std::min(y1, y2)) or (ray.origin.y > std::max(y1, y2)))
        {
            return 0;
        }
    }
    else
    {
        double t1 = (y1 - ray.origin.y)/ray.direction.y;
        double t2 = (y2 - ray.origin.y)/ray.direction.y;

        if (t1 > t2)
        {
            double tmp = t1;
            t1 = t2;
            t2 = tmp;
        }

        if ((t1 > tnear) or (tnear == -1))
        {
            tnear = t1;
        }

        if ((t2 < tfar) or (tfar == -1))
        {
            tfar = t2;
        }

        if (tnear > tfar)
        {
            //missed
            return 0;
        }

        if (tfar < 0)
        {
            //behind
            return 0;
        }
    }


    //dir z
    if (ray.direction.z == 0)
    {
        //std::cout << "in z" << std::endl;
        //parallel

        //outside
        if ((ray.origin.z < std::min(z1, z2)) or (ray.origin.z > std::max(z1, z2)))
        {
            return 0;
        }
    }
    else
    {
        double t1 = (z1 - ray.origin.z)/ray.direction.z;
        double t2 = (z2 - ray.origin.z)/ray.direction.z;

        if (t1 > t2)
        {
            double tmp = t1;
            t1 = t2;
            t2 = tmp;
        }

        if ((t1 > tnear) or (tnear == -1))
        {
            tnear = t1;
        }

        if ((t2 < tfar) or (tfar == -1))
        {
            tfar = t2;
        }

        if (tnear > tfar)
        {
            //missed
            return 0;
        }

        if (tfar < 0)
        {
            //behind
            return 0;
        }
    }

    double t = tnear;
    if (tnear < 0)
    {
        t = tfar;
    }

    Vec3 impact = ray.origin + ray.direction * t;
    //std::cout << "impact at" << impact.x << " " << impact.y << " " << impact.z << " dir is " << ray.direction.x << " " << ray.direction.y << " " << ray.direction.z <<  std::endl;

    if ((abs(impact.x - x1) < 10e-12) or (abs(impact.x - x2) < 10e-12))
    {
        //std::cout << "norm is x" << std::endl;
        minNorm = Vec3(1, 0, 0);
    }
    else if ((abs(impact.y - y1) < 10e-12) or (abs(impact.y - y2) < 10e-12))
    {
        //std::cout << "norm is y" << std::endl;
        minNorm = Vec3(0, 1, 0);
    }
    else if ((abs(impact.z - z1) < 10e-12) or (abs(impact.z - z2) < 10e-12))
    {
        //std::cout << "norm is z" << std::endl;
        minNorm = Vec3(0, 0, 1);
    }


    if (ray.direction.dot(minNorm) > 0)
    {
        minNorm *= -1;
    }

    normal = minNorm;

    return t;
}

double Box::getMinPos(const Vec3 axis) const
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

double Box::getMaxPos(const Vec3 axis) const
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
