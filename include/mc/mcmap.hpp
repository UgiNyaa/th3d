#ifndef MC_MAP_HPP
#define MC_MAP_HPP

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <mc/nbt/nbt.hpp>
#include <mc/nbt/compression.hpp>

#include <drawers/cubedrawer.hpp>

class MCMap
{
private:
    CubeDrawer cubedrawer;

    static const char *vertex_shader_code;
    static const char *fragment_shader_code;

    std::vector<glm::vec3> positions;

    void do_region(std::string path);
    void do_chunck(std::ifstream& in);
public:
    MCMap(std::string path);
    ~MCMap();

    void initialize();
    void draw(glm::mat4 view, glm::mat4 projection);
};

#endif /* end of include guard: MC_MAP_HPP */
