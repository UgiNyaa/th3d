#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Player {
private:
    GLFWwindow* window;
    glm::vec3 position;
    float horizontalAngle;
    float verticalAngle;
    float horizontalAngleNext;
    float verticalAngleNext;
    float deltatime;

    /* --- CALLBACKS --- */
    void cursor_position_callback
    (
        GLFWwindow* window,
        double xpos,
        double ypos
    );
    void scroll_callback
    (
        GLFWwindow* window,
        double xoffset,
        double yoffset
    );
    void key_callback
    (
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods
    );
public:
    Player (GLFWwindow* window);

    void set_deltatime(float t) { this->deltatime = t; }

    glm::mat4 model();

    void initialize();
    void update(double deltatime);
};

#endif /* end of include guard: PLAYER_HPP */
