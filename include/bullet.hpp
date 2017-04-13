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
    glm::vec3 pos;
    glm::vec3 player_dir;

    exprtk::expression<float> vel_x_expr;
    exprtk::expression<float> vel_y_expr;
    exprtk::expression<float> vel_z_expr;
    exprtk::expression<float> start_expr;

    int start() const
    {
        return static_cast<int>(start_expr.value());
    }

    void vel(float& x, float& y, float& z) const
    {
        x = vel_x_expr.value();
        y = vel_y_expr.value();
        z = vel_z_expr.value();
    }
};

#endif /* end of include guard: BULLET_HPP */
