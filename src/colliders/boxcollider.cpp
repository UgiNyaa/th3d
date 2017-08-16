#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>

#include "boxcollider.hpp"


bool BoxCollider::intersects
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
                static_cast<BoxCollider const&>(other),
                otherpos
            );
        break;

        default:
            return false;
    }
}

glm::vec3 BoxCollider::correct
(
    glm::vec3 const& thispos,
    Collider const& other,
    glm::vec3 const& otherpos,
    glm::vec3 const& othervel
)
{
    switch (other.collider_type()) {
        case ColliderType::BOX:
            return correct
            (
                thispos,
                static_cast<BoxCollider const&>(other),
                otherpos,
                othervel
            );
        break;

        default:
            return glm::vec3(1.0f);
    }
}

bool BoxCollider::intersects
(
    glm::vec3 const& thispos,
    BoxCollider const& other,
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

glm::vec3 BoxCollider::correct
(
    glm::vec3 const& thispos,
    BoxCollider const& other,
    glm::vec3 const& otherpos,
    glm::vec3 const& othervel
)
{
    auto amax = thispos + this->offset;
    auto amin = thispos - this->offset;
    auto bmax = otherpos + othervel + other.offset;
    auto bmin = otherpos + othervel - other.offset;

    bool intersects = amax.x > bmin.x
        && amax.y > bmin.y
        && amax.z > bmin.z
        && amin.x < bmax.x
        && amin.y < bmax.y
        && amin.z < bmax.z;

    if (!intersects)
        return othervel;

    auto oldbmin = bmin - othervel;
    auto oldbmax = bmax - othervel;
    bool use_amin = oldbmin.x < amin.x
        || oldbmin.y < amin.y
        || oldbmin.z < amin.z;
    bool use_amax = oldbmax.x > amax.x
        || oldbmax.y > amax.y
        || oldbmax.z > amax.z;

    auto newothervel = othervel;
    if (use_amax)
    {
        if (oldbmin.y > amax.y)
            newothervel.y += glm::abs(amax.y - bmin.y) + 0.01f;
        if (oldbmin.x > amax.x)
            newothervel.x += glm::abs(amax.x - bmin.x) + 0.01f;
        if (oldbmin.z > amax.z)
            newothervel.z += glm::abs(amax.z - bmin.z) + 0.01f;
    }

    if (use_amin)
    {
        if (oldbmax.y < amin.y)
            newothervel.y -= glm::abs(amin.y - bmax.y) + 0.01f;
        if (oldbmax.x < amin.x)
            newothervel.x -= glm::abs(amin.x - bmax.x) + 0.01f;
        if (oldbmax.z < amin.z)
            newothervel.z -= glm::abs(amin.z - bmax.z) + 0.01f;
    }

    return newothervel;
}


glm::vec3 BoxCollider::correct_old
(
    glm::vec3 const& thispos,
    BoxCollider const& other,
    glm::vec3 const& otherpos
)
{
    /*
    explanation of collision algorithm

    I wanted to develop a collision algorithm, that only needs the position
    of the one who collides into. My intention was to keep it simple. But it
    seems that this is not quite possible.

    Who does this method correct collision?

    the first thing it does, it checks with a simple algorithm, if the boxes
    intersect. If they dont, everything is fine, none has to change position.

    But if they do, the thing i first try to get an idea of, is wheather they
    collide on the max or min side. This is important, because when i correct
    the position, I have to know if I will have to use the min or max vector.
    The position will ultimately be aligned on either the x, y or z axis of the
    min or max.

    I keept it simple and the only thing i do, is comparing the position between
    the mins and the maxs. The lesser one will be used as the correction vector.

    The last thing left is to look for the axis the new position has to align
    to. This is also a very simple method. I just look for the smalest delta
    between the min and max axis.
    (if the min vector is choosen, I use the max vector of other, because it is
    the vector, that collides with the cube. You can draw 2 squares, define
    the min and max, and try to collide them. Then choose the min of max of
    sqare a by the rule above, and you will everytime see, that the oposite
    vector of b is the one intersecting with sqare a)
    After calculating the deltas, I just align it to the smallest. Why? Because
    when cube b collides with cube a, I have no idea where it comes from. so I
    just get the axis where it seems to travel the slowest and also intersecting
    the minimal amount. However, if it gets near an egde, it is uncertain if it
    came from one or another side from the edge. So it happes, that
    (espessially is your framerate is low) that you get "ported" to the other
    face of the edge. Same problem with points, but there you cam be "ported"
    to 2 other faces.

    So the quality of collision is determined by your framerate. The higher it
    is, the better the collision. But it never is perfect. You can imagine it
    as an asymptote. You must have an infinite framerate for perfect collision
    (because then the delta would be infinitly small and never be interrupted
    by the other delta getting smaller when nearing the edge or point).

    The other posibility is to determin the align axis by the direction the
    position want to travel to (with a velocity vector). It seems like the only
    solution.

    But since the collision I have now is almost perfect with high framerate,
    I will let it stay like that.
    */
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
    auto amax_to_bmin = bmin - amax;
    auto newotherpos = otherpos;

    if (glm::length2(amin_to_bmax) < glm::length2(amax_to_bmin))
    {
        // using amin as directive
        std::cout << "using amin" << '\n';
        auto amin_bmax_deltax = glm::max(bmax.x - amin.x, 0.0f);
        auto amin_bmax_deltay = glm::max(bmax.y - amin.y, 0.0f);
        auto amin_bmax_deltaz = glm::max(bmax.z - amin.z, 0.0f);
        std::cout << "deltax: " << amin_bmax_deltax << '\n';
        std::cout << "deltay: " << amin_bmax_deltay << '\n';
        std::cout << "deltaz: " << amin_bmax_deltaz << '\n';
        if (amin_bmax_deltay < amin_bmax_deltax)
        {
            if (amin_bmax_deltay < amin_bmax_deltaz)
            {
                // y is smallest
                std::cout << "y is smallest" << '\n';
                newotherpos.y = amin.y - other.offset.y;
            }
            else
            {
                // z is smallest
                std::cout << "z is smallest" << '\n';
                newotherpos.z = amin.z - other.offset.z;
            }
        }
        else
        {
            if (amin_bmax_deltax < amin_bmax_deltaz)
            {
                // x is smallest
                std::cout << "x is smallest" << '\n';
                newotherpos.x = amin.x - other.offset.x;
            }
            else
            {
                // z is smallest
                std::cout << "z is smallest" << '\n';
                newotherpos.z = amin.z - other.offset.z;
            }
        }
    }
    else
    {
        // using amax as directive
        std::cout << "using amax" << '\n';
        auto amax_bmin_deltax = glm::max(amax.x - bmin.x, 0.0f);
        auto amax_bmin_deltay = glm::max(amax.y - bmin.y, 0.0f);
        auto amax_bmin_deltaz = glm::max(amax.z - bmin.z, 0.0f);
        std::cout << "deltax: " << amax_bmin_deltax << '\n';
        std::cout << "deltay: " << amax_bmin_deltay << '\n';
        std::cout << "deltaz: " << amax_bmin_deltaz << '\n';
        if (amax_bmin_deltay < amax_bmin_deltax)
        {
            if (amax_bmin_deltay < amax_bmin_deltaz)
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
            if (amax_bmin_deltax < amax_bmin_deltaz)
            {
                // x is smallest
                newotherpos.x = amax.x + other.offset.x;
            }
            else
            {
                // z is smallest
                newotherpos.z = amax.z + other.offset.z;
            }
        }
    }

    return newotherpos;
}
