#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Box;

struct Box {
    glm::vec3 min, max;
    Box(glm::vec3 min, glm::vec3 max)
        : min(min), max(max) {}

    bool intersects(Box box);
};

#endif /* end of include guard: COLLISION_HPP */
