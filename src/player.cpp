#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <player.hpp>

Player::Player(GLFWwindow* window)
{
    this->window = window;
    this->horizontalAngleNext = 0;
    this->verticalAngleNext = 0;
    this->horizontalAngle = 0;
    this->verticalAngle = 0;
    this->position = glm::vec3(0,0,0);
}

void Player::initialize()
{
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);
}

void Player::cursor_position_callback
(
    GLFWwindow* window,
    double xpos,
    double ypos
)
{
    int32_t width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2, height/2);

    auto xdelta = float(width/2 - xpos);
    auto ydelta = float(height/2 - ypos);

    horizontalAngleNext += float(deltatime) * xdelta * 0.05f;
    verticalAngleNext += float(deltatime) * ydelta * 0.05f;
}

void Player::key_callback
(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
)
{
    bool go_forward     = key == GLFW_KEY_W && action == GLFW_PRESS;
    bool go_left        = key == GLFW_KEY_A && action == GLFW_PRESS;
    bool go_backward    = key == GLFW_KEY_S && action == GLFW_PRESS;
    bool go_right       = key == GLFW_KEY_D && action == GLFW_PRESS;
    bool go_up          = key == GLFW_KEY_E && action == GLFW_PRESS;
    bool go_down        = key == GLFW_KEY_Q && action == GLFW_PRESS;
    bool is_moving = go_forward || go_left || go_backward
        || go_right || go_up || go_down;

    if (is_moving)
    {
        verticalAngle = verticalAngleNext;
        horizontalAngle = horizontalAngleNext;

        glm::vec3 direction = glm::vec3(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );
        glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
        );
        glm::vec3 up = glm::cross(right, direction);

        glm::vec3 move =
            (go_forward ? direction : (go_backward ? -direction : glm::vec3(0, 0, 0)))
          + (go_right ? right : (go_left ? -right : glm::vec3(0, 0, 0)))
          + (go_up ? up : (go_down ? -up : glm::vec3(0, 0, 0)));
        move = glm::normalize(move);

        position += move * float(deltatime) * 10.0f;
    }
}
