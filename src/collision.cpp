#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>

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

glm::vec3 Box::correct
(
    glm::vec3 const& thispos,
    Collider const& other,
    glm::vec3 const& otherpos
)
{
    switch (other.collider_type()) {
        case ColliderType::BOX:
            return correct
            (
                thispos,
                static_cast<Box const&>(other),
                otherpos
            );
        break;

        default:
            return glm::vec3(1.0f);
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

glm::vec3 Box::correct
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

    bool intersects = amax.x > bmin.x
        && amax.y > bmin.y
        && amax.z > bmin.z
        && amin.x < bmax.x
        && amin.y < bmax.y
        && amin.z < bmax.z;

    if (!intersects)
        return otherpos;

    auto amin_to_bmax = bmax - amin;
    auto bmin_to_amax = amax - bmin;
    auto newotherpos = otherpos;

    if (glm::length2(amin_to_bmax) > glm::length2(bmin_to_amax))
    {
        // using amin as directive
        std::cout << "using amin" << '\n';
        if (amin_to_bmax.y < amin_to_bmax.x)
        {
            if (amin_to_bmax.y < amin_to_bmax.z)
            {
                // y is smallest
                newotherpos.y = amin.y - other.offset.y;
            }
            else
            {
                // z is smallest
                newotherpos.z = amin.z - other.offset.z;
            }
        }
        else
        {
            if (amin_to_bmax.x < amin_to_bmax.z)
            {
                // x is smallest
                newotherpos.x = amin.x - other.offset.x;
            }
        }
    }
    else
    {
        // using amax as directive
        std::cout << "using amax" << '\n';
        if (bmin_to_amax.y < bmin_to_amax.x)
        {
            if (bmin_to_amax.y < bmin_to_amax.z)
            {
                // y is smallest
                newotherpos.y = amax.y + other.offset.y;
            }
            else
            {
                // z is smallest
                newotherpos.z = amax.z + other.offset.z;
            }
        }
        else
        {
            if (bmin_to_amax.x < bmin_to_amax.z)
            {
                // x is smallest
                newotherpos.x = amax.x + other.offset.x;
            }
        }
    }

    return newotherpos;
}
