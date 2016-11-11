#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <game.hpp>
#include <camera.hpp>

Camera::Camera(Game* game)
{
    this->game = game;
    this->position = glm::vec3(0, 0, -5);
    this->horizontalAngle = 0;
    this->verticalAngle = 0;
    this->fov = 45;
}

glm::mat4 Camera::projection()
{
    return glm::perspective(
        glm::radians(this->fov),
        1280.0f / 720.0f,
        0.1f, 100.0f
    );
}

glm::mat4 Camera::view()
{
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

    return glm::lookAt(position, position + direction, up);
}

void Camera::update(double delta)
{
    double xpos, ypos;
    int32_t width, height;
    auto window = game->get_window();
    glfwGetWindowSize(window, &width, &height);
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, width/2, height/2);

    bool go_forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool go_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool go_backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool go_right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    bool go_up = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    bool go_down = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;

    auto xdelta = float(width/2 - xpos) * 0.003f;
    auto ydelta = float(height/2 - ypos) * 0.003f;
    horizontalAngle += float(delta) * xdelta;
    verticalAngle += float(delta) * ydelta;

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
    if (move != glm::vec3(0,0,0))
        move = glm::normalize(move);

    position += move * float(delta);
}
