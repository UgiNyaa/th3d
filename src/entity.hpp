#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Entity 
{
    const Shape* shape = nullptr;
    glm::vec3 position = glm::vec3(0, 0, 0);
};

#endif /* ENTITY_HPP */