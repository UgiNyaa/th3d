#ifndef BULLET_HPP
#define BULLET_HPP

#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <collision.hpp>
#include <player.hpp>
#include <time.hpp>
#include <shapes/shape.hpp>

class Bullet
{
private:
    const Time& t;
    const Shape* const shape;
public:
    Bullet (const Time& t, const Shape* const shape);

    void (*pos)(float& x, float& y, float& z, float t);

    glm::mat4 model();

    bool intersects(Player& player);

    void draw
    (
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    );
};

#endif /* end of include guard: BULLET_HPP */
