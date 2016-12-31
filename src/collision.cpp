#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <collision.hpp>

bool Box::intersects(Box box)
{
    auto a = *this;
    auto b = box;

    return a.max.x > b.min.x
        && a.max.y > b.min.y
        && a.max.z > b.min.z
        && a.min.x < b.max.x
        && a.min.y < b.max.y
        && a.min.z < b.max.z;
}
