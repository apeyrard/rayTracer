#include <ray.hpp>

namespace raytracer{

Ray::Ray(Vec3 origin, Vec3 direction)
    :origin(origin)
{
    this->direction = direction.norm();
}

}
