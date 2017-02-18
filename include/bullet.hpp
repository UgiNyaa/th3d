#ifndef BULLET_HPP
#define BULLET_HPP

#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <collision.hpp>
#include <player.hpp>
#include <shapes/shape.hpp>

class Bullet
{
private:
    glm::vec3 position;
    Shape const* const shape;
public:
    Bullet (Shape const* const shape);

    void (*pos)(float& x, float& y, float& z, float t);

    glm::mat4 model();

    std::function<float(float)> f_position_x;
    std::function<float(float)> f_position_y;
    std::function<float(float)> f_position_z;

    bool intersects(Player* player);

    void update(float deltatime);
    void draw
    (
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    );
};

#endif /* end of include guard: BULLET_HPP */
