#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Game;

class Camera
{
private:
    Game* game;
    glm::vec3 position;
    float horizontalAngle;
    float verticalAngle;
    float fov;
    float distance;
public:
    Camera ();
    Camera (Game* game);

    glm::mat4 projection();
    glm::mat4 view();
    glm::mat4 model();

    void update(double delta);
};

#endif /* end of include guard: CAMERA_HPP */
