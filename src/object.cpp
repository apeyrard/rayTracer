#include <object.hpp>

namespace raytracer{

Object::Object(Vec3 pos, Vec3 color, double reflection, double diffuse, double spec)
    : color(color)
    , reflection(reflection)
    , diffuse(diffuse)
    , spec(spec)
    , pos(pos)
{
}

Object::~Object()
{

}

}
