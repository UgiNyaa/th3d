#ifndef BULLET_HPP
#define BULLET_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <collision.hpp>

#include <functional>

class Bullet
{
private:
    glm::vec3 position;
public:
    Bullet ();

    glm::mat4 model();

    std::function<float(float)> f_position_x;
    std::function<float(float)> f_position_y;
    std::function<float(float)> f_position_z;

    Box get_collider();

    void update(float deltatime);
};

#endif /* end of include guard: BULLET_HPP */
