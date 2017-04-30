#include <games/testgame/testgame.hpp>

using json = nlohmann::json;

void TestGame::init_bmap(std::string json)
{
    auto j = json::parse(json);

    for (auto j_unit : j["units"])
    {
        auto u = new Unit();

        auto j_shape = j_unit["shape"].get<std::string>();
        for (size_t i = 0; i < shapes.size(); i++)
        {
            auto s = shapes[i];
            if (s->name() == j_shape)
            {
                u->shape = s;
                break;
            }
        }

        {
            auto pos_x_expr_str = j_unit["position"]["x"].get<std::string>();
            auto pos_y_expr_str = j_unit["position"]["y"].get<std::string>();
            auto pos_z_expr_str = j_unit["position"]["z"].get<std::string>();

            exprtk::symbol_table<float> symbol_table;
            symbol_table.add_constants();
            symbol_table.add_variable("t", t.full);

            u->pos_x_expr.register_symbol_table(symbol_table);
            u->pos_y_expr.register_symbol_table(symbol_table);
            u->pos_z_expr.register_symbol_table(symbol_table);

            exprtk::parser<float> parser;
            parser.compile(pos_x_expr_str, u->pos_x_expr);
            parser.compile(pos_y_expr_str, u->pos_y_expr);
            parser.compile(pos_z_expr_str, u->pos_z_expr);
        }

        for(auto j_engine : j_unit["engines"])
        {
            Shape const* shape;
            auto j_shape = j_engine["shape"].get<std::string>();
            for (size_t i = 0; i < shapes.size(); i++)
            {
                auto s = shapes[i];
                if (s->name() == j_shape)
                {
                    shape = s;
                    break;
                }
            }

            auto e = new Engine();

            {
                e->pos_x_expr_str = j_engine["position"]["x"].get<std::string>();
                e->pos_y_expr_str = j_engine["position"]["y"].get<std::string>();
                e->pos_z_expr_str = j_engine["position"]["z"].get<std::string>();

                e->n = j_engine["n"].get<int>();

                e->symbol_table.add_constants();
                e->symbol_table.add_constant("n", e->n);
                e->symbol_table.add_variable("t", t.full);

                e->shape = shape;
            }

            u->engines.push_back(e);
        }

        units.push_back(u);
    }
}
