#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <bullet.hpp>

struct Engine
{
    size_t n;
    std::string x_src;
    std::string y_src;
    std::string z_src;
    exprtk::symbol_table<float> symbol_table;

    void generate_into(std::vector<Bullet>& bullets)
    {
        for (size_t i = 0; i < n; i++)
        {
            auto b = new Bullet();

            b->vel_x_expr.register_symbol_table(symbol_table);
            b->vel_y_expr.register_symbol_table(symbol_table);
            b->vel_z_expr.register_symbol_table(symbol_table);
            b->start_expr.register_symbol_table(symbol_table);

            exprtk::parser<float> parser;
            parser.compile(x_src, b->vel_x_expr);
            parser.compile(y_src, b->vel_y_expr);
            parser.compile(z_src, b->vel_z_expr);
        }
    }
};

#endif /* end of include guard: ENGINE_HPP */
