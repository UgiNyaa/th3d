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
}

void Player::initialize()
{
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);
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

void Player::scroll_callback
(
    GLFWwindow* window,
    double xoffset,
    double yoffset
)
{

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

}
