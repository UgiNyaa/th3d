#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <camera.hpp>

Camera::Camera(Player* player)
{
    this->player = player;
    this->fov = 45;
    this->distance = 10.0f;
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
        cos(player->get_look_vertical_angle()) *
            sin(player->get_look_horizontal_angle()),

        sin(player->get_look_vertical_angle()),

        cos(player->get_look_vertical_angle()) *
            cos(player->get_look_horizontal_angle())
    );
    glm::vec3 right = glm::vec3(
        sin(player->get_look_horizontal_angle() - 3.14f/2.0f),
        0,
        cos(player->get_look_horizontal_angle() - 3.14f/2.0f)
    );
    glm::vec3 up = glm::cross(right, direction);

    return glm::lookAt(player->get_position() - (direction * distance), player->get_position() + direction, up);
}

void Camera::update(double deltatime)
{
    // double xpos, ypos;
    // int32_t width, height;
    // auto window = game->get_window();
    // glfwGetWindowSize(window, &width, &height);
    // glfwGetCursorPos(window, &xpos, &ypos);
    // glfwSetCursorPos(window, width/2, height/2);
    //
    // bool go_forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    // bool go_left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    // bool go_backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    // bool go_right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    // bool go_up = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    // bool go_down = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
    //
    // auto xdelta = float(width/2 - xpos);
    // auto ydelta = float(height/2 - ypos);
    // horizontalAngle += float(deltatime) * xdelta * 0.05f;
    // verticalAngle += float(deltatime) * ydelta * 0.05f;
    //
    // glm::vec3 direction = glm::vec3(
    //     cos(verticalAngle) * sin(horizontalAngle),
    //     sin(verticalAngle),
    //     cos(verticalAngle) * cos(horizontalAngle)
    // );
    // glm::vec3 right = glm::vec3(
    //     sin(horizontalAngle - 3.14f/2.0f),
    //     0,
    //     cos(horizontalAngle - 3.14f/2.0f)
    // );
    // glm::vec3 up = glm::cross(right, direction);
    //
    // glm::vec3 move =
    //     (go_forward ? direction : (go_backward ? -direction : glm::vec3(0, 0, 0)))
    //   + (go_right ? right : (go_left ? -right : glm::vec3(0, 0, 0)))
    //   + (go_up ? up : (go_down ? -up : glm::vec3(0, 0, 0)));
    // if (move != glm::vec3(0,0,0))
    //     move = glm::normalize(move);
    //
    // position += move * float(deltatime) * 10.0f;
}
