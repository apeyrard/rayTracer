#include <vec3.hpp>

#include <cmath>

namespace raytracer{

Vec3::Vec3()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Vec3::Vec3(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
{
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return Vec3(x+other.x, y+other.y, z+other.z);
}

void Vec3::operator+=(const Vec3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return Vec3(x-other.x, y-other.y, z-other.z);
}

void Vec3::operator-=(const Vec3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

Vec3 Vec3::operator*(double b) const
{
    return Vec3(x*b, y*b, z*b);
}

Vec3 Vec3::operator*(const Vec3& other) const
{
    return Vec3(x*other.x, y*other.y, z*other.z);
}

void Vec3::operator*=(double b)
{
    x*=b;
    y*=b;
    z*=b;
}

void Vec3::operator*=(const Vec3& other)
{
    x*=other.x;
    y*=other.y;
    z*=other.z;
}

Vec3 Vec3::operator/(double b) const
{
    return Vec3(x/b, y/b, z/b);
}

Vec3 Vec3::operator/(const Vec3& other) const
{
    return Vec3(x/other.x, y/other.y, z/other.z);
}

void Vec3::operator/=(double b)
{
    x/=b;
    y/=b;
    z/=b;
}

void Vec3::operator/=(const Vec3& other)
{
    x/=other.x;
    y/=other.y;
    z/=other.z;
}

Vec3 Vec3::norm()
{
    return *this * (1/sqrt(x*x+y*y+z*z));
}

double Vec3::dot(const Vec3& other) const
{
    return x*other.x + y*other.y + z*other.z;
}

Vec3 Vec3::cross(const Vec3& other) const
{
    return Vec3(y*other.z-z*other.y, z*other.x-x*other.z,x*other.y-y*other.x);
}


}
