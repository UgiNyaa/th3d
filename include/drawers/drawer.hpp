#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Drawer
{
public:
    virtual void initialize() {}
    virtual void draw
    (
        glm::mat4 model,
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    ) {};
};

#endif /* end of include guard: DRAWER_HPP */
