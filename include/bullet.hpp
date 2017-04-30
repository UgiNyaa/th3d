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

struct Bullet
{
    const Shape* shape;

    glm::vec3 unit_pos;

    exprtk::expression<float> pos_x_expr;
    exprtk::expression<float> pos_y_expr;
    exprtk::expression<float> pos_z_expr;

    Bullet()
        : shape(nullptr)
        , unit_pos(0, 0, 0)
    { }

    void pos(float& x, float& y, float& z) const
    {
        x = pos_x_expr.value() + unit_pos.x;
        y = pos_y_expr.value() + unit_pos.y;
        z = pos_z_expr.value() + unit_pos.z;
    }
};

#endif /* end of include guard: BULLET_HPP */
