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
    Collider() {}

    virtual bool intersects(const Collider* other) = 0;
    virtual ColliderType collider_type() const = 0;
};

struct Box : Collider {
    glm::vec3 min, max;
    Box(glm::vec3 min, glm::vec3 max)
        : min(min), max(max) {}

    bool intersects(const Collider* other) override;
    ColliderType collider_type() const override
        { return ColliderType::BOX; }
    bool intersects(const Box* other);
};

#endif /* end of include guard: COLLISION_HPP */
