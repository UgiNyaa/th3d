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
            auto pos_x = j_unit["position"]["x"].get<float>();
            auto pos_y = j_unit["position"]["y"].get<float>();
            auto pos_z = j_unit["position"]["z"].get<float>();

            u->pos = glm::vec3(pos_x, pos_y, pos_z);

            auto vel_x_expr_str = j_unit["velocity"]["x"].get<std::string>();
            auto vel_y_expr_str = j_unit["velocity"]["y"].get<std::string>();
            auto vel_z_expr_str = j_unit["velocity"]["z"].get<std::string>();

            exprtk::symbol_table<float> symbol_table;
            symbol_table.add_variable("t", t.full);
            symbol_table.add_variable("px", u->player_dir.x);
            symbol_table.add_variable("py", u->player_dir.y);
            symbol_table.add_variable("pz", u->player_dir.z);

            u->vel_x_expr.register_symbol_table(symbol_table);
            u->vel_y_expr.register_symbol_table(symbol_table);
            u->vel_z_expr.register_symbol_table(symbol_table);

            exprtk::parser<float> parser;
            parser.compile(vel_x_expr_str, u->vel_x_expr);
            parser.compile(vel_y_expr_str, u->vel_y_expr);
            parser.compile(vel_z_expr_str, u->vel_z_expr);
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
                e->x_src = j_engine["velocity"]["x"].get<std::string>();
                e->y_src = j_engine["velocity"]["y"].get<std::string>();
                e->z_src = j_engine["velocity"]["z"].get<std::string>();
                e->n = j_engine["n"].get<int>();

                exprtk::symbol_table<float> symbol_table;
                symbol_table.add_constants();
                symbol_table.add_variable("t", t.full);

                e->symbol_table = symbol_table;

                e->shape = shape;
                e->pos = u->pos;
            }

            u->engines.push_back(e);
        }

        for(auto j_bullet : j_unit["bullets"])
        {
            Shape const* shape;
            auto j_shape = j_bullet["shape"].get<std::string>();
            for (size_t i = 0; i < shapes.size(); i++)
            {
                auto s = shapes[i];
                if (s->name() == j_shape)
                {
                    shape = s;
                    break;
                }
            }

            auto b = new Bullet();

            {
                auto vel_x_expr_str = j_bullet["velocity"]["x"].get<std::string>();
                auto vel_y_expr_str = j_bullet["velocity"]["y"].get<std::string>();
                auto vel_z_expr_str = j_bullet["velocity"]["z"].get<std::string>();
                auto start_expr_str = j_bullet["start"].get<std::string>();

                exprtk::symbol_table<float> symbol_table;
                symbol_table.add_variable("t", t.full);
                symbol_table.add_variable("px", b->player_dir.x);
                symbol_table.add_variable("py", b->player_dir.y);
                symbol_table.add_variable("pz", b->player_dir.z);

                b->vel_x_expr.register_symbol_table(symbol_table);
                b->vel_y_expr.register_symbol_table(symbol_table);
                b->vel_z_expr.register_symbol_table(symbol_table);
                b->start_expr.register_symbol_table(symbol_table);

                exprtk::parser<float> parser;
                parser.compile(vel_x_expr_str, b->vel_x_expr);
                parser.compile(vel_y_expr_str, b->vel_y_expr);
                parser.compile(vel_z_expr_str, b->vel_z_expr);
                parser.compile(start_expr_str, b->start_expr);

                b->shape = shape;
                b->pos = u->pos;
            }

            u->bullets.push_back(b);
        }

        units.push_back(u);
    }
}
