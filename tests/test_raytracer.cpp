#include <gtest/gtest.h>
#include <cmath>
#include <vec3.hpp>
#include <ray.hpp>
#include <sphere.hpp>

using namespace raytracer;

TEST(vec3, addition)
{
    Vec3 a = Vec3(-5.3, 0, 4.333);
    Vec3 b = Vec3(12, 5.2, 0.000001);
    Vec3 c = a+b;
    EXPECT_DOUBLE_EQ(c.x, 6.7);
    EXPECT_DOUBLE_EQ(c.y, 5.2);
    EXPECT_DOUBLE_EQ(c.z, 4.333001);

    a += b;
    EXPECT_DOUBLE_EQ(a.x, 6.7);
    EXPECT_DOUBLE_EQ(a.y, 5.2);
    EXPECT_DOUBLE_EQ(a.z, 4.333001);
}

TEST(vec3, substraction)
{
    Vec3 a = Vec3(5, -1, 15.2);
    Vec3 b = Vec3(12, -5.2, 1);
    Vec3 c = a-b;
    EXPECT_DOUBLE_EQ(c.x, -7);
    EXPECT_DOUBLE_EQ(c.y, 4.2);
    EXPECT_DOUBLE_EQ(c.z, 14.2);

    a -= b;
    EXPECT_DOUBLE_EQ(a.x, -7);
    EXPECT_DOUBLE_EQ(a.y, 4.2);
    EXPECT_DOUBLE_EQ(a.z, 14.2);
}

TEST(vec3, doubleMultiplication)
{
    Vec3 a = Vec3(5, -1, 15.2);
    double b = 0.5;
    Vec3 c = a*b;
    EXPECT_DOUBLE_EQ(c.x, 2.5);
    EXPECT_DOUBLE_EQ(c.y, -0.5);
    EXPECT_DOUBLE_EQ(c.z, 7.6);

    a *= b;
    EXPECT_DOUBLE_EQ(a.x, 2.5);
    EXPECT_DOUBLE_EQ(a.y, -0.5);
    EXPECT_DOUBLE_EQ(a.z, 7.6);
}

TEST(vec3, componentMultiplication)
{
    Vec3 a = Vec3(5, -1, 15.2);
    Vec3 b = Vec3(12, -5.2, 1);
    Vec3 c = a*b;
    EXPECT_DOUBLE_EQ(c.x, 60);
    EXPECT_DOUBLE_EQ(c.y, 5.2);
    EXPECT_DOUBLE_EQ(c.z, 15.2);

    a *= b;
    EXPECT_DOUBLE_EQ(a.x, 60);
    EXPECT_DOUBLE_EQ(a.y, 5.2);
    EXPECT_DOUBLE_EQ(a.z, 15.2);
}

TEST(vec3, doubleDivision)
{
    Vec3 a = Vec3(5, -1, 15.2);
    double b = 3;
    Vec3 c = a/b;
    EXPECT_DOUBLE_EQ(c.x, 5.0/3.0);
    EXPECT_DOUBLE_EQ(c.y, -1.0/3.0);
    EXPECT_DOUBLE_EQ(c.z, 15.2/3.0);

    a /= b;
    EXPECT_DOUBLE_EQ(a.x, 5.0/3.0);
    EXPECT_DOUBLE_EQ(a.y, -1.0/3.0);
    EXPECT_DOUBLE_EQ(a.z, 15.2/3.0);
}

TEST(vec3, componentDivision)
{
    Vec3 a = Vec3(5, -1, 15.2);
    Vec3 b = Vec3(12, -5.2, 1);
    Vec3 c = a/b;
    EXPECT_DOUBLE_EQ(c.x, 5.0/12.0);
    EXPECT_DOUBLE_EQ(c.y, -1.0/-5.2);
    EXPECT_DOUBLE_EQ(c.z, 15.2);

    a /= b;
    EXPECT_DOUBLE_EQ(a.x, 5.0/12.0);
    EXPECT_DOUBLE_EQ(a.y, -1.0/-5.2);
    EXPECT_DOUBLE_EQ(a.z, 15.2);
}

TEST(vec3, norm)
{
    Vec3 a = Vec3(5, -1, 15.2);
    Vec3 b = a.norm();
    EXPECT_DOUBLE_EQ(b.x, a.x*1/(sqrt(a.x*a.x+a.y*a.y+a.z*a.z)));
    EXPECT_DOUBLE_EQ(b.y, a.y*1/(sqrt(a.x*a.x+a.y*a.y+a.z*a.z)));
    EXPECT_DOUBLE_EQ(b.z, a.z*1/(sqrt(a.x*a.x+a.y*a.y+a.z*a.z)));
    EXPECT_DOUBLE_EQ(sqrt(b.x*b.x + b.y*b.y + b.z*b.z), 1.0);
}

TEST(vec3, dotProduct)
{
    Vec3 a = Vec3(5, -1, 15.2);
    Vec3 b = Vec3(12, -5.2, 1);
    double c = a.dot(b);
    EXPECT_DOUBLE_EQ(c, 5*12+ 5.2 + 15.2);
}

TEST(vec3, crossProduct)
{
    Vec3 a = Vec3(1, 0, 0);
    Vec3 b = Vec3(0, 1, 0);
    Vec3 c = a.cross(b);
    EXPECT_DOUBLE_EQ(c.x, 0);
    EXPECT_DOUBLE_EQ(c.y, 0);
    EXPECT_DOUBLE_EQ(c.z, 1);
}

TEST(sphere, intersection)
{
    Vec3 n = Vec3();
    Sphere s = Sphere(Vec3(5, 0, 0), 1, Vec3(255, 255, 255));
    Ray r = Ray(Vec3(0, 0, 0), Vec3(1, 0, 0));

    //should hit at 4
    EXPECT_DOUBLE_EQ(s.intersect(r, n), 4.0);
    EXPECT_DOUBLE_EQ(n.x, -1);
    EXPECT_DOUBLE_EQ(n.y, 0);
    EXPECT_DOUBLE_EQ(n.z, 0);

    Ray r2 = Ray(Vec3(0, 0, 0), Vec3(2, 0, 0));

    //should hit at 4 too due to norm
    EXPECT_DOUBLE_EQ(s.intersect(r2, n), 4.0);
    EXPECT_DOUBLE_EQ(n.x, -1);
    EXPECT_DOUBLE_EQ(n.y, 0);
    EXPECT_DOUBLE_EQ(n.z, 0);

    Ray r3 = Ray(Vec3(0, 1, 0), Vec3(1, 0, 0));
    Ray r4 = Ray(Vec3(0, -1, 0), Vec3(1, 0, 0));
    Ray r5 = Ray(Vec3(0, 0, 1), Vec3(1, 0, 0));
    Ray r6 = Ray(Vec3(0, 0, -1), Vec3(1, 0, 0));

    //should hit at 5
    EXPECT_DOUBLE_EQ(s.intersect(r3, n), 5.0);
    EXPECT_DOUBLE_EQ(s.intersect(r4, n), 5.0);
    EXPECT_DOUBLE_EQ(s.intersect(r5, n), 5.0);
    EXPECT_DOUBLE_EQ(s.intersect(r6, n), 5.0);
    EXPECT_DOUBLE_EQ(n.x, 0);
    EXPECT_DOUBLE_EQ(n.y, 0);
    EXPECT_DOUBLE_EQ(n.z, -1);

    //should not hit
    Ray r7 = Ray(Vec3(0, 1.01, 0), Vec3(1, 0, 0));
    Ray r8 = Ray(Vec3(0, -1.01, 0), Vec3(1, 0, 0));
    Ray r9 = Ray(Vec3(0, 0, 1.01), Vec3(1, 0, 0));
    Ray r10 = Ray(Vec3(0, 0, -1.01), Vec3(1, 0, 0));
    EXPECT_DOUBLE_EQ(s.intersect(r7, n), 0);
    EXPECT_DOUBLE_EQ(s.intersect(r8, n), 0);
    EXPECT_DOUBLE_EQ(s.intersect(r9, n), 0);
    EXPECT_DOUBLE_EQ(s.intersect(r10, n), 0);
    // Thus norm shouldn't change
    EXPECT_DOUBLE_EQ(n.x, 0);
    EXPECT_DOUBLE_EQ(n.y, 0);
    EXPECT_DOUBLE_EQ(n.z, -1);

    Ray r11 = Ray(Vec3(12, 3.5, 2), Vec3(1, 1, 1));
    Sphere s2 = Sphere(Vec3(24, 15.5, 14), 10, Vec3(255, 255, 255));
    EXPECT_NE(s2.intersect(r11, n), 0);
    EXPECT_NE(n.x, 0);
    EXPECT_NE(n.y, 0);
    EXPECT_NE(n.z, 0);


}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
