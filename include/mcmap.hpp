#ifndef MC_MAP_HPP
#define MC_MAP_HPP

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class MCMap
{
private:
    static const char *vertex_shader_code;
    static const char *fragment_shader_code;

    std::vector<glm::vec3> vertex_buffer_data;
    std::vector<int> index_buffer_data;

    void do_region(std::string path);
    void do_chunck(std::ifstream& in);
    void do_section(std::ifstream& in);
public:
    MCMap(std::string path);
    ~MCMap();

    void initialize();
    void draw();
};

#endif /* end of include guard: MC_MAP_HPP */
