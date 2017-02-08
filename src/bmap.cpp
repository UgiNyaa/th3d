#include <functional>

#include <json.hpp>
#include <exprtk.hpp>

#include <bmap.hpp>

using json = nlohmann::json;

void BMap::update(float deltatime)
{
    gametime += deltatime;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(pos_data.get(), p, pos_data_size * 3 * 4);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    for(auto bullet : Bullets)
    {
        glUseProgram(bullet->computeShaderProgram);
        glUniform1f(0, gametime);

        glDispatchCompute(1, 1, 1);

        bullet->position = pos_data[0];
        //
        // glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }
}

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

BMap BMap::from_json_file
(
    std::string json_str,
    const std::vector<Shape*> shapes
)
{
    BMap bmap;
    std::map<std::string, GLuint> map;

    auto j = json::parse(json_str);

    bmap.pos_data_size = j["bullets"].size();
    bmap.pos_data = std::unique_ptr<glm::vec3[]>
    (
        new glm::vec3[bmap.pos_data_size]
    );

    std::cout << "float size: " << sizeof(GLfloat) << '\n';
    std::cout << "pos_data_size: " << bmap.pos_data_size << '\n';
    std::cout << "size: " << bmap.pos_data_size * 3 * 4 << '\n';

    glGenBuffers(1, &bmap.ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bmap.ssbo);
    glBufferData
    (
        GL_SHADER_STORAGE_BUFFER,
        bmap.pos_data_size * 3 * 4,
        bmap.pos_data.get(),
        GL_DYNAMIC_COPY
    );
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bmap.ssbo);

    for (auto j_file : j["scripts"])
    {
        auto file = j_file.get<std::string>();
        std::cout << file << '\n';
        std::ifstream ifs(file);
        std::string code
        (
            (std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>()
        );

        auto pID = glCreateProgram();
        auto csID = glCreateShader(GL_COMPUTE_SHADER);

        // buffer initialization
        GLuint block_index = glGetProgramResourceIndex
        (
            pID,
            GL_SHADER_STORAGE_BLOCK,
            "output_buffer_data"
        );
        std::cout << block_index << '\n';
        glShaderStorageBlockBinding(pID, block_index, 0);

        // int* params;
        // glGetProgramiv(pID, GL_ACTIVE_UNIFORM_BLOCKS, params);
        // std::cout << *params << '\n';

        // shader compiling + program linking
        auto c_code = code.c_str();
        glShaderSource(csID, 1, &c_code, NULL);
        glCompileShader(csID);
        int rvalue;
        glGetShaderiv(csID, GL_COMPILE_STATUS, &rvalue);
        if (!rvalue)
        {
            fprintf(stderr, "Error in compiling the compute shader\n");
            GLchar log[10240];
            GLsizei length;
            glGetShaderInfoLog(csID, 10239, &length, log);
            fprintf(stderr, "Compiler log:\n%s\n", log);
            exit(40);
        }

        glAttachShader(pID, csID);
        glLinkProgram(pID);
        glGetProgramiv(pID, GL_LINK_STATUS, &rvalue);
        if (!rvalue) {
            fprintf(stderr, "Error in linking compute shader program\n");
            GLchar log[10240];
            GLsizei length;
            glGetProgramInfoLog(pID, 10239, &length, log);
            fprintf(stderr, "Linker log:\n%s\n", log);
            exit(41);
        }
        glUseProgram(pID);

        map.insert(std::make_pair(file, pID));
    }
    for (auto j_bullet : j["bullets"])
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

        auto bullet = new Bullet(shape);
        bullet->computeShaderProgram = map[j_bullet["position"]];

        // auto j_x = j_bullet["x"];
        // bullet->f_position_x = math_expr_to_func2(j_x.get<std::string>().c_str());
        //
        // auto j_y = j_bullet["y"];
        // bullet->f_position_y = math_expr_to_func2(j_y.get<std::string>().c_str());
        //
        // auto j_z = j_bullet["z"];
        // bullet->f_position_z = math_expr_to_func2(j_z.get<std::string>().c_str());

        bmap.Bullets.push_back(bullet);
    }

    return bmap;
}
