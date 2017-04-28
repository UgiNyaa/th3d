#ifndef UNIT_HPP
#define UNIT_HPP

#include <shapes/shape.hpp>
#include <bullet.hpp>
#include <engine.hpp>

struct Unit
{
    const Shape* shape;
    glm::vec3 pos;
    glm::vec3 player_dir;

    std::vector<Bullet*> bullets;
    std::vector<Engine*> engines;
    exprtk::expression<float> vel_x_expr;
    exprtk::expression<float> vel_y_expr;
    exprtk::expression<float> vel_z_expr;

    void vel(float& x, float& y, float& z) const
    {
        x = vel_x_expr.value();
        y = vel_y_expr.value();
        z = vel_z_expr.value();
    }
};

#endif /* end of include guard: UNIT_HPP */
