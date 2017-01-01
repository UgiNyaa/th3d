#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <collision.hpp>

class Shape {
public:
    virtual void draw
    (
        glm::mat4 model,
        glm::mat4 view,
        glm::mat4 projection
    ) = 0;
};

#endif /* end of include guard: SHAPE_HPP */
