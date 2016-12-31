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
    glm::vec3 position;
    std::vector<Bullet*> bullets;
    float bodyHorizontalAngle;
    float bodyVerticalAngle;
    float lookHorizontalAngle;
    float lookVerticalAngle;
public:
    Player (GLFWwindow* window, std::vector<Bullet*> bullets);

    glm::vec3 get_position() { return position; }
    float get_look_horizontal_angle() { return lookHorizontalAngle; }
    float get_look_vertical_angle() { return lookVerticalAngle; }

    glm::mat4 model();

    void update(double deltatime);
};

#endif /* end of include guard: PLAYER_HPP */
