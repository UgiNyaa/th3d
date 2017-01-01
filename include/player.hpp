#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <bullet.hpp>
#include <shapes/shape.hpp>
#include <collision.hpp>

class Player {
private:
    GLFWwindow* window;
    Shape * const shape;
    glm::vec3 position;
    float bodyHorizontalAngle;
    float bodyVerticalAngle;
    float lookHorizontalAngle;
    float lookVerticalAngle;
public:
    Player
    (
        GLFWwindow* window,
        Shape * const shape
    );

    glm::vec3 get_position() { return position; }
    float get_look_horizontal_angle() { return lookHorizontalAngle; }
    float get_look_vertical_angle() { return lookVerticalAngle; }

    glm::mat4 model();

    void update(double deltatime);
    void draw(glm::mat4 view, glm::mat4 projection);
};

#endif /* end of include guard: PLAYER_HPP */
