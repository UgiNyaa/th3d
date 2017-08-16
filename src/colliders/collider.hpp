#ifndef COLLIDER_HPP
#define COLLIDER_HPP

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
    virtual glm::vec3 correct
    (
        glm::vec3 const& thispos,
        Collider const& other,
        glm::vec3 const& otherpos,
        glm::vec3 const& othervel
    ) = 0;
};


#endif /* end of include guard: COLLIDER_HPP */
