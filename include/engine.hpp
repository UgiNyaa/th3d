#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>

#include <exprtk.hpp>
#include <bullet.hpp>

struct Engine
{
    const Shape* shape;

    size_t n;
    size_t r;

    glm::vec3 unit_pos;

    exprtk::symbol_table<float> symbol_table;
    std::string pos_x_expr_str;
    std::string pos_y_expr_str;
    std::string pos_z_expr_str;

    Engine()
        : shape(nullptr)
        , n(0)
        , r(0)
        , unit_pos(0, 0, 0)
        , pos_x_expr_str("")
        , pos_y_expr_str("")
        , pos_z_expr_str("")
    { }

    void generate_into(std::vector<Bullet*>& bullets)
    {
        symbol_table.add_constant("i", 0);
        for (size_t i = 0; i < n; i++)
        {
            auto b = new Bullet();

            symbol_table.remove_variable("i");
            symbol_table.add_constant("i", i+1);

            b->pos_x_expr.register_symbol_table(symbol_table);
            b->pos_y_expr.register_symbol_table(symbol_table);
            b->pos_z_expr.register_symbol_table(symbol_table);

            exprtk::parser<float> parser;
            parser.compile(pos_x_expr_str, b->pos_x_expr);
            parser.compile(pos_y_expr_str, b->pos_y_expr);
            parser.compile(pos_z_expr_str, b->pos_z_expr);

            b->shape = shape;
            b->unit_pos = unit_pos;

            bullets.push_back(b);
        }
    }
};

#endif /* end of include guard: ENGINE_HPP */
