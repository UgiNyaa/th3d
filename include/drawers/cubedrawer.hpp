#ifndef CUBEDRAWER_HPP
#define CUBEDRAWER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <drawers/drawer.hpp>

class CubeDrawer : public Drawer
{
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
    ~CubeDrawer();

    void initialize() override;
    void draw
    (
        glm::mat4 model,
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    ) override;
};

#endif /* end of include guard: CUBEDRAWER_HPP */
