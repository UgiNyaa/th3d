#ifndef UNIT_HPP
#define UNIT_HPP

#include <shapes/shape.hpp>
#include <bullet.hpp>
#include <engine.hpp>

struct Unit
{
    const Shape* shape;

    std::vector<Bullet*> bullets;
    std::vector<Engine*> engines;
    exprtk::expression<float> pos_x_expr;
    exprtk::expression<float> pos_y_expr;
    exprtk::expression<float> pos_z_expr;

    void pos(float& x, float& y, float& z) const
    {
        x = pos_x_expr.value();
        y = pos_y_expr.value();
        z = pos_z_expr.value();
    }
};

#endif /* end of include guard: UNIT_HPP */
