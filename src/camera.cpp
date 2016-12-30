#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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
}

void Camera::on_scroll
(
    GLFWwindow* window,
    double xoffset,
    double yoffset
)
{
    distance += yoffset;
    if (distance < 0)
        distance = 0;
}
