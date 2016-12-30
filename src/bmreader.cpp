#include <bmreader.hpp>
#include <json.hpp>
using json = nlohmann::json;
#include <functional>
#include <exprtk.hpp>
#include <string>

std::function<float(float)> math_expr_to_func(const char* math_expr)
{
    float t;

    exprtk::symbol_table<float> symbol_table;
    symbol_table.add_variable("t", t);
    symbol_table.add_constants();

    exprtk::expression<float> expression;
    expression.register_symbol_table(symbol_table);

    exprtk::parser<float> parser;
    parser.compile(math_expr, expression);

    return [=, &t](float time) -> float{
        t = time;
        return expression.value();
    };
}

std::vector<Bullet*> bm_json_read(const char* bm_json)
{
    auto bullets = std::vector<Bullet*>();
    auto j = json::parse(bm_json);
    for (auto j_bullet : j)
    {
        auto bullet = new Bullet();

        auto j_x = j_bullet["x"];
        bullet->f_position_x = math_expr_to_func(j_x.get<std::string>().c_str());

        auto j_y = j_bullet["y"];
        bullet->f_position_y = math_expr_to_func(j_y.get<std::string>().c_str());

        auto j_z = j_bullet["z"];
        bullet->f_position_z = math_expr_to_func(j_z.get<std::string>().c_str());

        bullets.push_back(bullet);
    }

    return bullets;
}
