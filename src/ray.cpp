#include <ray.hpp>

namespace raytracer{

Ray::Ray(Vec3 origin, Vec3 direction, int depth, double refr)
    :origin(origin)
    , depth(depth)
    , refr(refr) // refraction indices of current medium
{
    this->direction = direction.norm();
}

}
