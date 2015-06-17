#include <cylinder.hpp>
#include <cmath>
#include <iostream>

namespace raytracer{

Cylinder::Cylinder(Vec3 pos, Vec3 dir, double length, double radius, Vec3 color, double reflection, double diffuse, double spec, double refr, double light)
    : Object(pos, color, reflection, diffuse, spec, refr, light)
    , radius(radius)
    , length(length)
{
    direction = dir.norm();
    bottom = new Plane(direction, -direction.dot(pos) + length/2.0, color, reflection, diffuse, spec, light);
    top = new Plane(direction, -direction.dot(pos) - length/2.0, color, reflection, diffuse, spec, light);
}

Cylinder::~Cylinder()
{
    delete top;
    delete bottom;
}

double Cylinder::intersect(const Ray& ray, Vec3& normal) const
{
    Vec3 a = (ray.direction - direction * (ray.direction.dot(direction)));
    double A = a.dot(a);

    //std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
    Vec3 deltaPos = ray.origin - pos;
    double B = 2.0*( (ray.direction - direction * (ray.direction.dot(direction))).dot(deltaPos - direction * (deltaPos.dot(direction))));

    Vec3 c = (deltaPos - direction*(deltaPos.dot(direction)));
    double C = c.dot(c) - pow(radius, 2);

    // Now solving At**2 +Bt +C = 0
    double delta = pow(B, 2) - 4.0*A*C;

    //t0 and t1 intersection with cylinder
    double t0 = 0;
    double t1 = 0;

    //t2, t3 intersections with plane
    double t2 = 0;
    double t3 = 0;

    if (delta >= 0)
    {
        // if negative discriminant, no solution to the equation, no intersection

        t0 = (-B - sqrt(delta))/(2.0*A);
        if (t0 > 0)
        {
            //closest intersection is t0
            normal = Vec3(0, 1, 0);
        }

        t1 = (-B + sqrt(delta))/(2.0*A);
        if (t1 > 0)
        {
            //closest intersection is t1
            normal = Vec3(1, 0, 0);
        }

    }

    // useful for verifications
    Vec3 bottomCenter = pos - direction * length/2.0;
    Vec3 topCenter = pos + direction * length/2.0;

    // verifying t0 and t1
    Vec3 t0InterPoint = ray.origin + ray.direction * t0;
    Vec3 t1InterPoint = ray.origin + ray.direction * t1;

    Vec3 t0norm = t0InterPoint - (pos + direction * (t0InterPoint - pos).dot(direction));
    Vec3 t1norm = t1InterPoint - (pos + direction * (t1InterPoint - pos).dot(direction));

    if (std::abs((t0InterPoint - pos).dot(direction)) > length/2.0)
    {

        t0 = 0;
    }

    if (std::abs((t1InterPoint - pos).dot(direction)) > length/2.0)
    {
        t1 = 0;
    }

    Vec3 topNormal = Vec3();
    t2 = top->intersect(ray, topNormal);

    Vec3 bottomNormal = Vec3();
    t3 = bottom->intersect(ray, bottomNormal);

    double bottomDist = ((ray.origin + ray.direction * t3) - bottomCenter).length();
    double topDist = ((ray.origin + ray.direction * t2) - topCenter).length();

    if (bottomDist > radius)
    {
        t3 = 0;
    }

    if (topDist > radius)
    {
        t2 = 0;
    }

    // now we return the minimum positive t, with its normal
    //std::cout << t0 << " " << t1 << " " << t2 << " " << t3 << std::endl;
    double tmin = 0;
    if (t0 > 0)
    {
        normal = t0norm.norm();
        tmin = t0;
    }

    if ((t1 > 0) and ((t1 < tmin) or (tmin == 0)))
    {
        normal = t1norm.norm();
        tmin = t1;
    }

    if ((t2 > 0) and ((t2 < tmin) or (tmin == 0)))
    {
        normal = topNormal.norm();
        tmin = t2;
    }

    if ((t3 > 0) and ((t3 < tmin) or (tmin == 0)))
    {
        normal = bottomNormal.norm();
        tmin = t3;
    }

    if (ray.direction.dot(normal) > 0)
    {
        normal *= -1;
    }

    return tmin;
}

Vec3 Cylinder::getRandPoint(std::default_random_engine &rng) const
{
    return Vec3();
}

double Cylinder::getMinPos(const Vec3 axis) const
{
    return 0;
}

double Cylinder::getMaxPos(const Vec3 axis) const
{
    return 0;
}

}
