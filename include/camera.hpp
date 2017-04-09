#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <player.hpp>

struct Camera
{
    float field_of_view;
    float distance;

    Camera()
        : field_of_view(45.0f)
        , distance(10.0f)
    { }

};

// class Camera
// {
// private:
//     Player& player;
//     float fov;
//     float distance;
// public:
//     Camera ();
//     Camera (Player& player);
//
//     glm::mat4 projection();
//     glm::mat4 view();
//
//     void update(double delta);
//
//     void on_scroll
//     (
//         GLFWwindow* window,
//         double xoffset,
//         double yoffset
//     );
// };

#endif /* end of include guard: CAMERA_HPP */
