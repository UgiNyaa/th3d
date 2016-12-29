#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <player.hpp>

class Camera
{
private:
    Player* player;
    float fov;
    float distance;
public:
    Camera ();
    Camera (Player* player);

    glm::mat4 projection();
    glm::mat4 view();

    void update(double delta);
};

#endif /* end of include guard: CAMERA_HPP */
