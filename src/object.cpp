#include <object.hpp>

namespace raytracer{

Object::Object(Vec3 pos, Vec3 color, double reflection, double diffuse)
    : color(color)
    , reflection(reflection)
    , diffuse(diffuse)
    , pos(pos)
{
}

Object::~Object()
{

}

}
