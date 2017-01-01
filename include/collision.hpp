#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Box;

enum struct ColliderType
{
    BOX
};

struct Collider {
    virtual ColliderType collider_type() const = 0;
    virtual bool intersects
    (
        glm::vec3 const& thispos,
        Collider const& other,
        glm::vec3 const& otherpos
    ) = 0;
};

struct Box : public Collider {
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
    bool intersects
    (
        glm::vec3 const& thispos,
        Box const& other,
        glm::vec3 const& otherpos
    );
};

#endif /* end of include guard: COLLISION_HPP */
