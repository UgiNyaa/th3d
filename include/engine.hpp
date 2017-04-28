#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>

#include <bullet.hpp>

struct Engine
{
    const Shape* shape;
    glm::vec3 pos;
    size_t n;
    std::string x_src;
    std::string y_src;
    std::string z_src;
    exprtk::symbol_table<float> symbol_table;

    void replace_everything(std::string& str, const std::string& from, const std::string& to)
    {
        if(from.empty())
            return;
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    void generate_into(std::vector<Bullet*>& bullets)
    {
        symbol_table.add_constant("n", 0);
        for (size_t i = 0; i < n; i++)
        {
            auto b = new Bullet();

            symbol_table.remove_variable("n");
            symbol_table.add_constant("n", i+1);

            b->vel_x_expr.register_symbol_table(symbol_table);
            b->vel_y_expr.register_symbol_table(symbol_table);
            b->vel_z_expr.register_symbol_table(symbol_table);
            b->start_expr.register_symbol_table(symbol_table);

            exprtk::parser<float> parser;
            parser.compile(x_src, b->vel_x_expr);
            parser.compile(y_src, b->vel_y_expr);
            parser.compile(z_src, b->vel_z_expr);

            b->shape = shape;
            b->pos = pos;

            bullets.push_back(b);
        }
    }
};

#endif /* end of include guard: ENGINE_HPP */
