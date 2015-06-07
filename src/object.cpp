#include <object.hpp>

namespace raytracer{

Object::Object(Vec3 pos, Vec3 color, double reflection, double diffuse, double spec, double refr, double light)
    : color(color)
    , reflection(reflection)
    , diffuse(diffuse)
    , spec(spec)
    , refr(refr)
    , light(light)
    , pos(pos)
{
}

Object::~Object()
{

}

}
