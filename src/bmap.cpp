// #include <json.hpp>
// #include <bmap.hpp>
// #include <functional>
// #include <exprtk.hpp>
//
// using json = nlohmann::json;
//
// std::function<float(float)> math_expr_to_func2(const char* math_expr)
// {
//     float t;
//
//     exprtk::symbol_table<float> symbol_table;
//     symbol_table.add_variable("t", t);
//     symbol_table.add_constants();
//
//     exprtk::expression<float> expression;
//     expression.register_symbol_table(symbol_table);
//
//     exprtk::parser<float> parser;
//     parser.compile(math_expr, expression);
//
//     return [=, &t](float time) -> float
//     {
//         t = time;
//         return expression.value();
//     };
// }
//
// BMap& read_bmap_json(std::string json_str)
// {
//     BMap bmap;
//     auto j = json::parse(json_str);
//     for (auto j_bullet : j)
//     {
//         Shape const* shape;
//         auto j_shape = j_bullet["shape"].get<std::string>();
//         for (size_t i = 0; i < (sizeof(bmap_shapes)/sizeof(*bmap_shapes)); i++)
//         {
//             auto s = (bmap_shapes + i);
//             if (s->name() == j_shape)
//             {
//                 shape = s;
//                 break;
//             }
//         }
//
//         auto bullet = new Bullet(shape);
//
//         auto j_x = j_bullet["x"];
//         bullet->f_position_x = math_expr_to_func2(j_x.get<std::string>().c_str());
//
//         auto j_y = j_bullet["y"];
//         bullet->f_position_y = math_expr_to_func2(j_y.get<std::string>().c_str());
//
//         auto j_z = j_bullet["z"];
//         bullet->f_position_z = math_expr_to_func2(j_z.get<std::string>().c_str());
//
//         bmap.bullets.push_back(bullet);
//     }
//
//     return bmap;
// }
