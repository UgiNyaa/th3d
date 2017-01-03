#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <shapes/shape.hpp>
#include <collision.hpp>

class Player {
private:
    GLFWwindow* window;
    float bodyHorizontalAngle;
    float bodyVerticalAngle;
    float lookHorizontalAngle;
    float lookVerticalAngle;
    Box box;
public:
    Shape * const shape;
    glm::vec3 position;

    Player
    (
        GLFWwindow* window,
        Shape * const shape
    );

    Collider& get_collider() { return box; }
    glm::vec3 get_position() { return position; }
    float get_look_horizontal_angle() { return lookHorizontalAngle; }
    float get_look_vertical_angle() { return lookVerticalAngle; }

    glm::mat4 model();

    void update(double deltatime);
    void draw
    (
        glm::mat4 view,
        glm::mat4 projection,
        glm::vec3 colourmultiplier
    );
};

#endif /* end of include guard: PLAYER_HPP */
