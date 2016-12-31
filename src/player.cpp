#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <player.hpp>
#include <collision.hpp>

Player::Player(GLFWwindow* window, std::vector<Bullet*> bullets)
{
    this->window = window;
    this->bullets = bullets;
    this->bodyHorizontalAngle = 0;
    this->bodyVerticalAngle = 0;
    this->lookHorizontalAngle = 0;
    this->lookVerticalAngle = 0;
    this->position = glm::vec3(0,0,5);
}

glm::mat4 Player::model()
{
    return
        glm::translate(position) *
        glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));
}

void Player::update(double deltatime)
{
    double xpos, ypos;
    int32_t width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwGetCursorPos(window, &xpos, &ypos);

    bool go_forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool go_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool go_backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool go_right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool go_up = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    bool go_down = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
    bool moving = go_forward || go_backward
        || go_left || go_right
        || go_up || go_down;

    auto xdelta = float(width/2 - xpos);
    auto ydelta = float(height/2 - ypos);
    lookHorizontalAngle += float(deltatime) * xdelta * 0.05f;
    lookVerticalAngle += float(deltatime) * ydelta * 0.05f;

    if (moving)
    {
        bodyHorizontalAngle = lookHorizontalAngle;
        bodyVerticalAngle = lookVerticalAngle;
        glm::vec3 direction = glm::vec3
        (
            cos(bodyVerticalAngle) * sin(bodyHorizontalAngle),
            sin(bodyVerticalAngle),
            cos(bodyVerticalAngle) * cos(bodyHorizontalAngle)
        );
        glm::vec3 right = glm::vec3
        (
            sin(bodyHorizontalAngle - 3.14f/2.0f),
            0,
            cos(bodyHorizontalAngle - 3.14f/2.0f)
        );
        glm::vec3 up = glm::cross(right, direction);

        glm::vec3 move =
            (go_forward ? direction : (go_backward ? -direction : glm::vec3(0, 0, 0)))
          + (go_right ? right : (go_left ? -right : glm::vec3(0, 0, 0)))
          + (go_up ? up : (go_down ? -up : glm::vec3(0, 0, 0)));
        move = glm::normalize(move);

        auto velocity = move * float(deltatime) * 10.0f;

        // future intersection detection

        position += velocity;
    }
}
