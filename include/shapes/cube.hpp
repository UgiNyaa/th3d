#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <string>

#include <shapes/shape.hpp>
#include <collision.hpp>

class Cube : public Shape {
private:
    static const GLfloat g_vertex_buffer_data[];
    static const GLfloat g_uv_buffer_data[];
    static const char *vertex_shader_code;
    static const char *fragment_shader_code;

    GLuint pID;
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint mvpID;
    GLuint colourmultiplierID;
public:
    Cube();
    ~Cube();

    std::string name() override { return "Cube"; }
    void draw
    (
        glm::mat4 model,
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    ) override;
};

#endif /* end of include guard: CUBE_HPP */
