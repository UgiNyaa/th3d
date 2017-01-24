#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <string>

#include <collision.hpp>
#include <colliders/collider.hpp>

struct Shape
{
    virtual std::string name() = 0;
    virtual Collider& collider() = 0;
    virtual void draw
    (
        glm::mat4 model,
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    ) = 0;
};

#endif /* end of include guard: SHAPE_HPP */
