#include <ray.hpp>

namespace raytracer{

Ray::Ray(Vec3 origin, Vec3 direction, int depth)
    :origin(origin)
    , depth(depth)
{
    this->direction = direction.norm();
}

}
