#pragma once

namespace raytracer{

class Vec3
{
public:
    double x;
    double y;
    double z;
    Vec3();
    Vec3(double x, double y, double z);
    Vec3 operator+(const Vec3& other) const;
    void operator+=(const Vec3& other);
    Vec3 operator-(const Vec3& other) const;
    void operator-=(const Vec3& other);
    Vec3 operator*(double b) const;
    Vec3 operator*(const Vec3& other) const;
    void operator*=(double b);
    void operator*=(const Vec3& other);
    Vec3 operator/(double b) const;
    Vec3 operator/(const Vec3& other) const;
    void operator/=(double b);
    void operator/=(const Vec3& other);
    Vec3 norm();
    double dot(const Vec3& other) const;
    Vec3 cross(const Vec3& other) const;
    double length() const;
};

}
