#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <collision.hpp>

bool Box::intersects(const Collider* other)
{
    switch (other->collider_type())
    {
        case ColliderType::BOX:
            return intersects(static_cast<const Box*>(other));
        break;

        default:
            return false;
    }
}

bool Box::intersects(const Box* other)
{
    auto a = this;
    auto b = other;

    return a->max.x > b->min.x
        && a->max.y > b->min.y
        && a->max.z > b->min.z
        && a->min.x < b->max.x
        && a->min.y < b->max.y
        && a->min.z < b->max.z;
}
