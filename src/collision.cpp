#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <collision.hpp>

bool Box::intersects
(
    glm::vec3 const& thispos,
    Collider const& other,
    glm::vec3 const& otherpos
)
{
    switch (other.collider_type())
    {
        case ColliderType::BOX:
            return intersects
            (
                thispos,
                static_cast<Box const&>(other),
                otherpos
            );
        break;

        default:
            return false;
    }
}

bool Box::intersects
(
    glm::vec3 const& thispos,
    Box const& other,
    glm::vec3 const& otherpos
)
{
    auto amax = thispos + this->offset;
    auto amin = thispos - this->offset;
    auto bmax = otherpos + other.offset;
    auto bmin = otherpos - other.offset;

    return amax.x > bmin.x
        && amax.y > bmin.y
        && amax.z > bmin.z
        && amin.x < bmax.x
        && amin.y < bmax.y
        && amin.z < bmax.z;
}
