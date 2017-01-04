#ifndef BOXCOLLIDER_HPP
#define BOXCOLLIDER_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct BoxCollider : public Collider
{
    glm::vec3 offset;
    Box(glm::vec3 offset)
        : offset(offset) {}

    ColliderType collider_type() const override
        { return ColliderType::BOX; }

    bool intersects
    (
        glm::vec3 const& thispos,
        Collider const& other,
        glm::vec3 const& otherpos
    ) override;
    glm::vec3 correct
    (
        glm::vec3 const& thispos,
        Collider const& other,
        glm::vec3 const& otherpos,
        glm::vec3 const& othervel
    );
    bool intersects
    (
        glm::vec3 const& thispos,
        Box const& other,
        glm::vec3 const& otherpos
    );
    glm::vec3 correct
    (
        glm::vec3 const& thispos,
        Box const& other,
        glm::vec3 const& otherpos,
        glm::vec3 const& othervel
    );


    glm::vec3 correct_old
    (
        glm::vec3 const& thispos,
        Box const& other,
        glm::vec3 const& otherpos
    );
};

#endif /* end of include guard: BOXCOLLIDER_HPP */
