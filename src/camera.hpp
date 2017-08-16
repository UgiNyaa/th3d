#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "player.hpp"

struct Camera
{
    float field_of_view;
    float distance;

    Camera()
        : field_of_view(45.0f)
        , distance(10.0f)
    { }

};

#endif /* end of include guard: CAMERA_HPP */
